#include <gmsh.h>
#include <cmath>

int main(int argc, char **argv) {
    gmsh::initialize();
    gmsh::model::add("torus");
    
    gmsh::option::setNumber("Geometry.OCCBooleanPreserveNumbering", 1);
    
    double R = 10.0;  
    double r_outer = 4;  
    double r_inner = 2.5;  
    
    gmsh::model::occ::addTorus(0, 0, 0, R, r_outer, 1);
    gmsh::model::occ::addTorus(0, 0, 0, R, r_inner, 2);
    
    std::vector<std::pair<int, int>> out;
    std::vector<std::vector<std::pair<int, int>>> out_map;
    gmsh::model::occ::cut({{3, 1}}, {{3, 2}}, out, out_map, 3);
    
    gmsh::model::occ::synchronize();
    
    gmsh::option::setNumber("Mesh.CharacteristicLengthMin", 0.3);
    gmsh::option::setNumber("Mesh.CharacteristicLengthMax", 0.7);
    
    gmsh::model::mesh::generate(3);
    
    gmsh::write("_No_1.msh");
    
    
    gmsh::finalize();
    return 0;
}