#include "poisson.h"
#include <basix/finite-element.h>
#include <cmath>
#include <dolfinx.h>
#include <dolfinx/fem/Constant.h>
#include <dolfinx/fem/petsc.h>
#include <dolfinx/la/petsc.h>
#include <dolfinx/mesh/utils.h>
#include <petscmat.h>
#include <petscsys.h>
#include <utility>
#include <vector>
#include <memory>
#include <functional>

using namespace dolfinx;
using T = PetscScalar;
using U = typename dolfinx::scalar_value_t<T>;

int main(int argc, char* argv[])
{
  dolfinx::init_logging(argc, argv);
  PetscInitialize(&argc, &argv, nullptr, nullptr);

  {
    // =====================================================
    // 1. Создаем полную сетку
    // =====================================================
    auto part = mesh::create_cell_partitioner(mesh::GhostMode::shared_facet);
    
    auto full_mesh = std::make_shared<mesh::Mesh<U>>(
        mesh::create_rectangle<U>(MPI_COMM_WORLD, {{{0.0, 0.0}, {1.0, 1.0}}},
                                  {64, 64}, mesh::CellType::triangle, part));

    // =====================================================
    // 2. Находим ячейки ВНЕ отверстия (по ЦЕНТРОИДУ!)
    // =====================================================
    constexpr U hole_min = 0.35;
    constexpr U hole_max = 0.65;
    
    auto topology = full_mesh->topology();
    topology->create_connectivity(2, 0);
    
    auto x = full_mesh->geometry().x();
    //const std::size_t gdim = full_mesh->geometry().dim();
    auto x_dofmap = full_mesh->geometry().dofmap();
    const std::size_t nodes_per_cell = x_dofmap.extent(1);
    
    std::vector<std::int32_t> cells_to_keep;
    /*
    for (std::int32_t c = 0; c < topology->index_map(2)->size_local(); ++c)
    {
      // Вычисляем ЦЕНТРОИД ячейки
      U cx = 0.0;
      U cy = 0.0;
      
      for (std::size_t local_i = 0; local_i < nodes_per_cell; ++local_i)
      {
        std::int32_t node_idx = x_dofmap(c, local_i);
        cx += x[node_idx*3 + 0];
        cy += x[node_idx*3 + 1];
      }

      cx /= static_cast<U>(nodes_per_cell);
      cy /= static_cast<U>(nodes_per_cell);
      
      // Сохраняем ячейку, если её центроид ВНЕ отверстия
      if (cx < hole_min || cx > hole_max || cy < hole_min || cy > hole_max)
      {
        cells_to_keep.push_back(c);
      }
    }*/

    for (std::int32_t c = 0; c < topology->index_map(2)->size_local(); ++c)
{
  // Проверяем ВСЕ вершины ячейки
  bool all_vertices_outside = true;
  
  for (std::size_t local_i = 0; local_i < nodes_per_cell; ++local_i)
  {
    std::int32_t node_idx = x_dofmap(c, local_i);
    U x0 = x[node_idx * 3 + 0];
    U x1 = x[node_idx * 3 + 1];
    
    // Если ХОТЯ БЫ ОДНА вершина ВНУТРИ отверстия - удаляем ячейку
    if (x0 > hole_min && x0 < hole_max && x1 > hole_min && x1 < hole_max)
    {
      all_vertices_outside = false;
      break;
    }
  }
  
  if (all_vertices_outside)
    cells_to_keep.push_back(c);
}
    
    // Создаем под-сетку
    auto [mesh_data, vertex_map, edge_map, cell_map] = 
        mesh::create_submesh(*full_mesh, 2, cells_to_keep);
    auto mesh = std::make_shared<mesh::Mesh<U>>(std::move(mesh_data));

    // =====================================================
    // 3. Пространство функций
    // =====================================================
    auto element = basix::create_element<U>(
        basix::element::family::P, basix::cell::type::triangle, 1,
        basix::element::lagrange_variant::unset,
        basix::element::dpc_variant::unset, false);

    auto V = std::make_shared<fem::FunctionSpace<U>>(
        fem::create_functionspace<U>(
            mesh, std::make_shared<fem::FiniteElement<U>>(element)));

    // =====================================================
    // 4. Коэффициенты и формы
    // =====================================================
    auto kappa = std::make_shared<fem::Constant<T>>(2.0);
    auto f = std::make_shared<fem::Function<T>>(V);

    std::vector<std::shared_ptr<const fem::FunctionSpace<U>>> spaces_a = {V, V};
    std::vector<std::shared_ptr<const fem::FunctionSpace<U>>> spaces_L = {V};
    
    std::map<std::string, std::shared_ptr<const fem::Constant<T>>> constants_a = {{"kappa", kappa}};
    std::map<std::string, std::shared_ptr<const fem::Function<T>>> functions_L = {{"f", f}};

    fem::Form<T> a = fem::create_form<T>(*form_poisson_a, spaces_a, {}, constants_a, {}, {});
    fem::Form<T> L = fem::create_form<T>(*form_poisson_L, spaces_L, functions_L, {}, {}, {});

    // =====================================================
    // 5. Граничные условия Дирихле на ВСЕХ границах
    // =====================================================
    std::vector<std::int32_t> facets = mesh::locate_entities_boundary(
        *mesh, 1,
        [](auto x) -> std::vector<std::int8_t>
        {
          std::vector<std::int8_t> marker(x.extent(1), true);
          return marker;
        });
    
    std::vector<std::int32_t> bdofs = fem::locate_dofs_topological(
        *V->mesh()->topology_mutable(), *V->dofmap(), 1, facets);
    
    fem::DirichletBC<T> bc(static_cast<T>(0.0), bdofs, V);

    // =====================================================
    // 6. Интерполяция источника f
    // =====================================================
    f->interpolate(
        [](auto x) -> std::pair<std::vector<T>, std::vector<std::size_t>>
        {
          std::vector<T> f_vals;
          for (std::size_t p = 0; p < x.extent(1); ++p)
          {
            auto dx = (x(0, p) - 0.5) * (x(0, p) - 0.5);
            auto dy = (x(1, p) - 0.5) * (x(1, p) - 0.5);
            //f_vals.push_back(10 * std::exp(-(dx + dy) / 0.02));
            f_vals.push_back(100 * std::cos(-(2*dx + dy) / 0.025));
          }
          return {f_vals, {f_vals.size()}};
        });

    // =====================================================
    // 7. Сборка и решение
    // =====================================================
    auto u = std::make_shared<fem::Function<T>>(V);
    
    la::petsc::Matrix A(fem::petsc::create_matrix(a), false);
    la::Vector<T> b(L.function_spaces()[0]->dofmap()->index_map,
                    L.function_spaces()[0]->dofmap()->index_map_bs());

    MatZeroEntries(A.mat());
    fem::assemble_matrix(la::petsc::Matrix::set_block_fn(A.mat(), ADD_VALUES),
                         a, {bc});
    MatAssemblyBegin(A.mat(), MAT_FLUSH_ASSEMBLY);
    MatAssemblyEnd(A.mat(), MAT_FLUSH_ASSEMBLY);
    fem::set_diagonal<T>(la::petsc::Matrix::set_fn(A.mat(), INSERT_VALUES), *V, {bc});
    MatAssemblyBegin(A.mat(), MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A.mat(), MAT_FINAL_ASSEMBLY);

    std::ranges::fill(b.array(), 0);
    fem::assemble_vector(b.array(), L);
    fem::apply_lifting(b.array(), {a}, {{bc}}, {}, T(1));
    b.scatter_rev(std::plus<T>());
    bc.set(b.array(), std::nullopt);

    la::petsc::KrylovSolver lu(MPI_COMM_WORLD);
    la::petsc::options::set("ksp_type", "preonly");
    la::petsc::options::set("pc_type", "lu");
    lu.set_from_options();
    lu.set_operator(A.mat());
    
    la::petsc::Vector _u(la::petsc::create_vector_wrap(*u->x()), false);
    la::petsc::Vector _b(la::petsc::create_vector_wrap(b), false);
    lu.solve(_u.vec(), _b.vec());

    u->x()->scatter_fwd();

    // =====================================================
    // 8. Сохранение результата
    // =====================================================
    io::VTKFile file(MPI_COMM_WORLD, "u.pvd", "w");
    std::vector<std::reference_wrapper<const fem::Function<T>>> u_vector = {*u};
    file.write(u_vector, 0.0);
  }

  PetscFinalize();
  return 0;
}