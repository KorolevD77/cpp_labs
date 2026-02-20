#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTetra.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>

#include <gmsh.h>

using namespace std;

// Класс расчётной точки
class CalcNode
{
// Класс сетки будет friend-ом точки
friend class CalcMesh;

protected:
    // Координаты
    
    // Некая величина, в попугаях
    double smth;
    // Скорость
    

public:
    double vx;
    double vy;
    double vz;
    double x;
    double y;
    double z;
    // Конструктор по умолчанию
    CalcNode() : x(0.0), y(0.0), z(0.0), smth(0.0), vx(0.0), vy(0.0), vz(0.0)
    {
    }

    // Конструктор с указанием всех параметров
    CalcNode(double x, double y, double z, double smth, double vx, double vy, double vz) 
            : x(x), y(y), z(z), smth(smth), vx(vx), vy(vy), vz(vz)
    {
    }

    // Метод отвечает за перемещение точки
    // Движемся время tau из текущего положения с текущей скоростью
    void move(double tau) {
        x += vx * tau;
        y += vy * tau;
        z += vz * tau;
    }
};

// Класс элемента сетки
class Element
{
// Класс сетки будет friend-ом и элемента тоже
// (и вообще будет нагло считать его просто структурой)
friend class CalcMesh;

protected:
    // Индексы узлов, образующих этот элемент сетки
    unsigned long nodesIds[4];
};

// Класс расчётной сетки
class CalcMesh
{
protected:
    // 3D-сетка из расчётных точек
    vector<CalcNode> nodes;
    vector<Element> elements;

    // Вспомогательная структура для bounding box
    struct BoundingBox {
        double minX, maxX;
        double minY, maxY;
        double minZ, maxZ;
        
        double centerX() const { return (minX + maxX) / 2.0; }
        double centerY() const { return (minY + maxY) / 2.0; }
        double centerZ() const { return (minZ + maxZ) / 2.0; }
    };

    // Функция для вычисления bounding box по координатам узлов
    BoundingBox computeBoundingBox(const std::vector<double>& nodesCoords) {
        BoundingBox bb;
        
        if (nodesCoords.empty()) {
            bb.minX = bb.maxX = bb.minY = bb.maxY = bb.minZ = bb.maxZ = 0.0;
            return bb;
        }
        
        bb.minX = bb.maxX = nodesCoords[0];
        bb.minY = bb.maxY = nodesCoords[1];
        bb.minZ = bb.maxZ = nodesCoords[2];
        
        for (size_t i = 0; i < nodesCoords.size(); i += 3) {
            bb.minX = std::min(bb.minX, nodesCoords[i]);
            bb.maxX = std::max(bb.maxX, nodesCoords[i]);
            bb.minY = std::min(bb.minY, nodesCoords[i + 1]);
            bb.maxY = std::max(bb.maxY, nodesCoords[i + 1]);
            bb.minZ = std::min(bb.minZ, nodesCoords[i + 2]);
            bb.maxZ = std::max(bb.maxZ, nodesCoords[i + 2]);
        }
        
        return bb;
    }

public:
    // Конструктор сетки из заданного stl-файла с центрированием
    CalcMesh(const std::vector<double>& nodesCoords, const std::vector<std::size_t>& tetrsPoints) {
        
        // Вычисляем bounding box для центрирования
        BoundingBox bb = computeBoundingBox(nodesCoords);
        
        // Вычисляем центр модели
        double centerX = bb.centerX();
        double centerY = bb.centerY();
        double centerZ = bb.centerZ();
        
        cout << "Исходные границы модели:" << endl;
        cout << "  X: [" << bb.minX << ", " << bb.maxX << "]" << endl;
        cout << "  Y: [" << bb.minY << ", " << bb.maxY << "]" << endl;
        cout << "  Z: [" << bb.minZ << ", " << bb.maxZ << "]" << endl;
        cout << "Центр модели: (" << centerX << ", " << centerY << ", " << centerZ << ")" << endl;
        cout << "Размеры модели: " << (bb.maxX - bb.minX) << " x " 
             << (bb.maxY - bb.minY) << " x " << (bb.maxZ - bb.minZ) << endl;
        
        // Пройдём по узлам в модели gmsh и центрируем их
        nodes.resize(nodesCoords.size() / 3);
        for(unsigned int i = 0; i < nodesCoords.size() / 3; i++) {
            // Координаты заберём из gmsh и центрируем
            double pointX = nodesCoords[i*3] - centerX;
            double pointY = nodesCoords[i*3 + 1] - centerY;
            double pointZ = nodesCoords[i*3 + 2] - centerZ;
            
            // Модельная скалярная величина распределена как-то вот так
            double smth = sqrt(pow(pointX, 2) + pow(pointY, 2) + pow(pointZ, 2));
            
            // Скорость - оставим как была или изменим для центрированной модели
            double vx = pointX;  // Скорость пропорциональна координате
            double vy = pointY;
            double vz = 50.0;   // Постоянная скорость по Z
            
            nodes[i] = CalcNode(pointX, pointY, pointZ, smth, vx, vy, vz);
        }

        // Пройдём по элементам в модели gmsh
        elements.resize(tetrsPoints.size() / 4);
        for(unsigned int i = 0; i < tetrsPoints.size() / 4; i++) {
            elements[i].nodesIds[0] = tetrsPoints[i*4] - 1;
            elements[i].nodesIds[1] = tetrsPoints[i*4 + 1] - 1;
            elements[i].nodesIds[2] = tetrsPoints[i*4 + 2] - 1;
            elements[i].nodesIds[3] = tetrsPoints[i*4 + 3] - 1;
        }
        
        // Выводим информацию о центрированной модели
        BoundingBox newBB = computeBoundingBoxFromNodes();
        cout << "После центрирования:" << endl;
        cout << "  Новый центр: (0, 0, 0)" << endl;
        cout << "  Новые границы: X[" << newBB.minX << ", " << newBB.maxX 
             << "], Y[" << newBB.minY << ", " << newBB.maxY 
             << "], Z[" << newBB.minZ << ", " << newBB.maxZ << "]" << endl;
    }
    
    // Альтернативный конструктор с возможностью выбора типа центрирования
    enum CenterType {
        CENTER_BY_BBOX,  // Центрировать по bounding box
        CENTER_BY_MASS,  // Центрировать по центру масс (если нужна физическая точность)
        NO_CENTERING     // Не центрировать
    };
    
    CalcMesh(const std::vector<double>& nodesCoords, const std::vector<std::size_t>& tetrsPoints, CenterType centerType) {
        double offsetX = 0, offsetY = 0, offsetZ = 0;
        
        if (centerType == CENTER_BY_BBOX) {
            BoundingBox bb = computeBoundingBox(nodesCoords);
            offsetX = bb.centerX();
            offsetY = bb.centerY();
            offsetZ = bb.centerZ();
        }
        else if (centerType == CENTER_BY_MASS) {
            // Вычисление центра масс (можно реализовать при необходимости)
            // Это более сложный вариант, требует учета объемов элементов
            cout << "Центрирование по центру масс пока не реализовано, использую BBOX" << endl;
            BoundingBox bb = computeBoundingBox(nodesCoords);
            offsetX = bb.centerX();
            offsetY = bb.centerY();
            offsetZ = bb.centerZ();
        }
        
        // Создание узлов с центрированием
        nodes.resize(nodesCoords.size() / 3);
        for(unsigned int i = 0; i < nodesCoords.size() / 3; i++) {
            double pointX = nodesCoords[i*3] - offsetX;
            double pointY = nodesCoords[i*3 + 1] - offsetY;
            double pointZ = nodesCoords[i*3 + 2] - offsetZ;
            
            double smth = sqrt(pow(pointX, 2) + pow(pointY, 2) + pow(pointZ, 2));
            
            nodes[i] = CalcNode(pointX, pointY, pointZ, smth, -pointY*5, pointY*5, 80.0);
        }
        
        // Создание элементов
        elements.resize(tetrsPoints.size() / 4);
        for(unsigned int i = 0; i < tetrsPoints.size() / 4; i++) {
            elements[i].nodesIds[0] = tetrsPoints[i*4] - 1;
            elements[i].nodesIds[1] = tetrsPoints[i*4 + 1] - 1;
            elements[i].nodesIds[2] = tetrsPoints[i*4 + 2] - 1;
            elements[i].nodesIds[3] = tetrsPoints[i*4 + 3] - 1;
        }
    }
    
    // Вспомогательная функция для вычисления bounding box по текущим узлам
    BoundingBox computeBoundingBoxFromNodes() {
        BoundingBox bb;
        
        if (nodes.empty()) {
            bb.minX = bb.maxX = bb.minY = bb.maxY = bb.minZ = bb.maxZ = 0.0;
            return bb;
        }
        
        bb.minX = bb.maxX = nodes[0].x;
        bb.minY = bb.maxY = nodes[0].y;
        bb.minZ = bb.maxZ = nodes[0].z;
        
        for (const auto& node : nodes) {
            bb.minX = std::min(bb.minX, node.x);
            bb.maxX = std::max(bb.maxX, node.x);
            bb.minY = std::min(bb.minY, node.y);
            bb.maxY = std::max(bb.maxY, node.y);
            bb.minZ = std::min(bb.minZ, node.z);
            bb.maxZ = std::max(bb.maxZ, node.z);
        }
        
        return bb;
    }

    // Метод отвечает за выполнение для всей сетки шага по времени величиной tau
    void doTimeStep(double tau) {
        // По сути метод просто двигает все точки
        for(unsigned int i = 0; i < nodes.size(); i++) {
            nodes[i].move(tau);

            nodes[i].vx = -nodes[i].y*5;
            nodes[i].vy = nodes[i].x*5;
            nodes[i].vz = 100*tau+50;
        }
    }

    // Метод отвечает за запись текущего состояния сетки в снапшот в формате VTK
    void snapshot(unsigned int snap_number) {
        // Сетка в терминах VTK
        vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        // Точки сетки в терминах VTK
        vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();

        // Скалярное поле на точках сетки
        auto smth = vtkSmartPointer<vtkDoubleArray>::New();
        smth->SetName("smth");

        // Векторное поле на точках сетки
        auto vel = vtkSmartPointer<vtkDoubleArray>::New();
        vel->SetName("velocity");
        vel->SetNumberOfComponents(3);

        // Обходим все точки нашей расчётной сетки
        for(unsigned int i = 0; i < nodes.size(); i++) {
            // Вставляем новую точку в сетку VTK-снапшота
            dumpPoints->InsertNextPoint(nodes[i].x, nodes[i].y, nodes[i].z);

            // Добавляем значение векторного поля в этой точке
            double _vel[3] = {nodes[i].vx, nodes[i].vy, nodes[i].vz};
            vel->InsertNextTuple(_vel);

            // И значение скалярного поля тоже
            smth->InsertNextValue(nodes[i].smth);
        }

        // Грузим точки в сетку
        unstructuredGrid->SetPoints(dumpPoints);

        // Присоединяем векторное и скалярное поля к точкам
        unstructuredGrid->GetPointData()->AddArray(vel);
        unstructuredGrid->GetPointData()->AddArray(smth);

        // А теперь пишем, как наши точки объединены в тетраэдры
        for(unsigned int i = 0; i < elements.size(); i++) {
            auto tetra = vtkSmartPointer<vtkTetra>::New();
            tetra->GetPointIds()->SetId( 0, elements[i].nodesIds[0] );
            tetra->GetPointIds()->SetId( 1, elements[i].nodesIds[1] );
            tetra->GetPointIds()->SetId( 2, elements[i].nodesIds[2] );
            tetra->GetPointIds()->SetId( 3, elements[i].nodesIds[3] );
            unstructuredGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
        }

        // Создаём снапшот в файле с заданным именем
        string fileName = "propeller-" + std::to_string(snap_number) + ".vtu";
        vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(unstructuredGrid);
        writer->Write();
    }
};

int main()
{
    // Шаг точек по пространству
    double h = 4.0;
    // Шаг по времени
    double tau = 0.01;

    const unsigned int GMSH_TETR_CODE = 4;

    // Теперь придётся немного упороться:
    // (а) построением сетки средствами gmsh,
    // (б) извлечением данных этой сетки в свой код.
    gmsh::initialize();
    gmsh::model::add("t13");

    // Считаем STL
    try {
        gmsh::merge("../../lab1/t13_data.stl"); 
        // путь к файлу отсчитывается от точки запуска 
        // если вы собирали все в директории build как цивилизованные люди, 
        // то перейдите на уровень выше
        // и запускайте бинарник как ./build/tetr3d
    } catch(...) {
        gmsh::logger::write("Could not load STL mesh: bye!");
        gmsh::finalize();
        return -1;
    }

    // Восстановим геометрию
    double angle = 40;
    bool forceParametrizablePatches = false;
    bool includeBoundary = true;
    double curveAngle = 180;
    gmsh::model::mesh::classifySurfaces(angle * M_PI / 180., includeBoundary, forceParametrizablePatches, curveAngle * M_PI / 180.);
    gmsh::model::mesh::createGeometry();

    // Зададим объём по считанной поверхности
    std::vector<std::pair<int, int> > s;
    gmsh::model::getEntities(s, 2);
    std::vector<int> sl;
    for(auto surf : s) sl.push_back(surf.second);
    int l = gmsh::model::geo::addSurfaceLoop(sl);
    gmsh::model::geo::addVolume({l});

    gmsh::model::geo::synchronize();

    // Зададим мелкость желаемой сетки
    int f = gmsh::model::mesh::field::add("MathEval");
    gmsh::model::mesh::field::setString(f, "F", "4");
    gmsh::model::mesh::field::setAsBackgroundMesh(f);

    // Построим сетку
    gmsh::model::mesh::generate(3);

    // Теперь извлечём из gmsh данные об узлах сетки
    std::vector<double> nodesCoord;
    std::vector<std::size_t> nodeTags;
    std::vector<double> parametricCoord;
    gmsh::model::mesh::getNodes(nodeTags, nodesCoord, parametricCoord);

    // И данные об элементах сетки тоже извлечём, нам среди них нужны только тетраэдры, которыми залит объём
    std::vector<std::size_t>* tetrsNodesTags = nullptr;
    std::vector<int> elementTypes;
    std::vector<std::vector<std::size_t>> elementTags;
    std::vector<std::vector<std::size_t>> elementNodeTags;
    gmsh::model::mesh::getElements(elementTypes, elementTags, elementNodeTags);
    for(unsigned int i = 0; i < elementTypes.size(); i++) {
        if(elementTypes[i] != GMSH_TETR_CODE)
            continue;
        tetrsNodesTags = &elementNodeTags[i];
    }

    if(tetrsNodesTags == nullptr) {
        cout << "Can not find tetra data. Exiting." << endl;
        gmsh::finalize();
        return -2;
    }

    cout << "The model has " <<  nodeTags.size() << " nodes and " << tetrsNodesTags->size() / 4 << " tetrs." << endl;

    // На всякий случай проверим, что номера узлов идут подряд и без пробелов
    for(int i = 0; i < nodeTags.size(); ++i) {
        // Индексация в gmsh начинается с 1, а не с нуля. Ну штош, значит так.
        assert(i == nodeTags[i] - 1);
    }
    // И ещё проверим, что в тетраэдрах что-то похожее на правду лежит.
    assert(tetrsNodesTags->size() % 4 == 0);

    // TODO: неплохо бы полноценно данные сетки проверять, да

    // Создаем сетку с центрированием (используем первый конструктор)
    CalcMesh mesh(nodesCoord, *tetrsNodesTags);
    
    // Альтернативно можно использовать второй конструктор с явным указанием типа центрирования:
    // CalcMesh mesh(nodesCoord, *tetrsNodesTags, CalcMesh::CENTER_BY_BBOX);

    gmsh::finalize();

    mesh.snapshot(0);
    for(unsigned int step = 1; step < 100; step++) {
        mesh.doTimeStep(tau);
        mesh.snapshot(step);
    }

    return 0;
}