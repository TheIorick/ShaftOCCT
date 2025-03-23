/**
 * @file ShaftBuilder.h
 * @brief Класс для построения 3D модели вала с фасками
 */

#ifndef SHAFT_BUILDER_H
#define SHAFT_BUILDER_H

#include <BRepPrimAPI_MakeCylinder.hxx>  // Создание цилиндров
#include <BRepPrimAPI_MakeCone.hxx>      // Создание конусов
#include <BRepPrimAPI_MakeBox.hxx>       // Создание прямоугольных параллелепипедов
#include <BRepAlgoAPI_Fuse.hxx>          // Операция объединения тел
#include <BRepAlgoAPI_Cut.hxx>           // Операция вычитания тел
#include <STEPControl_Writer.hxx>        // Запись в формат STEP
#include <TopoDS_Shape.hxx>              // Базовый класс для топологических объектов
#include <gp_Ax2.hxx>                    // Ось для построения геометрических примитивов
#include <gp_Pnt.hxx>                    // Точка в 3D пространстве
#include <gp_Dir.hxx>                    // Направление в 3D пространстве
#include <BRepFilletAPI_MakeChamfer.hxx> // Создание фасок
#include <BRepFilletAPI_MakeFillet.hxx>  // Создание скруглений
#include <TopExp_Explorer.hxx>           // Обход топологических элементов
#include <TopTools_IndexedMapOfShape.hxx>// Коллекция топологических объектов
#include <TopExp.hxx>                    // Топологические операции
#include <TopoDS.hxx>                    // Приведение типов для топологических объектов
#include <BRep_Tool.hxx>                 // Инструменты для работы с геометрией
#include <BRepBndLib.hxx>                // Работа с ограничивающими боксами
#include <Bnd_Box.hxx>                   // Ограничивающий бокс
#include <iostream>
#include <vector>
#include <memory>
#include <string>

/**
 * @class ShaftSegment
 * @brief Базовый класс для сегментов вала
 */
class ShaftSegment {
protected:
    Standard_Real zStart;    // Начальная координата Z сегмента
    Standard_Real length;    // Длина сегмента
    Standard_Real radius;    // Радиус сегмента

public:
    /**
     * @brief Конструктор сегмента вала
     * @param zStart Начальная координата Z
     * @param length Длина сегмента
     * @param radius Радиус сегмента
     */
    ShaftSegment(Standard_Real zStart, Standard_Real length, Standard_Real radius)
        : zStart(zStart), length(length), radius(radius) {}

    virtual ~ShaftSegment() = default;

    /**
     * @brief Создать геометрическую форму сегмента
     * @return Форма сегмента
     */
    virtual TopoDS_Shape create() const = 0;

    // Геттеры
    Standard_Real getZStart() const { return zStart; }
    Standard_Real getLength() const { return length; }
    Standard_Real getRadius() const { return radius; }
    Standard_Real getZEnd() const { return zStart + length; }
};

/**
 * @class CylinderSegment
 * @brief Класс для цилиндрических сегментов вала
 */
class CylinderSegment : public ShaftSegment {
public:
    /**
     * @brief Конструктор цилиндрического сегмента
     * @param zStart Начальная координата Z
     * @param length Длина сегмента
     * @param diameter Диаметр сегмента
     */
    CylinderSegment(Standard_Real zStart, Standard_Real length, Standard_Real diameter)
        : ShaftSegment(zStart, length, diameter / 2.0) {}

    /**
     * @brief Создать цилиндрическую форму
     * @return Форма цилиндра
     */
    TopoDS_Shape create() const override {
        // Создаем ось для цилиндра
        gp_Ax2 axis(gp_Pnt(0.0, 0.0, zStart), gp_Dir(0.0, 0.0, 1.0));
        // Создаем и возвращаем цилиндр
        return BRepPrimAPI_MakeCylinder(axis, radius, length).Shape();
    }
};

/**
 * @class ConeSegment
 * @brief Класс для конических сегментов вала
 */
class ConeSegment : public ShaftSegment {
private:
    Standard_Real radiusEnd;  // Радиус на конце конуса

public:
    /**
     * @brief Конструктор конического сегмента
     * @param zStart Начальная координата Z
     * @param length Длина сегмента
     * @param diameterStart Диаметр начала сегмента
     * @param diameterEnd Диаметр конца сегмента
     */
    ConeSegment(Standard_Real zStart, Standard_Real length,
                Standard_Real diameterStart, Standard_Real diameterEnd)
        : ShaftSegment(zStart, length, diameterStart / 2.0),
        radiusEnd(diameterEnd / 2.0) {}

    /**
     * @brief Создать коническую форму
     * @return Форма конуса
     */
    TopoDS_Shape create() const override {
        // Создаем ось для конуса
        gp_Ax2 axis(gp_Pnt(0.0, 0.0, zStart), gp_Dir(0.0, 0.0, 1.0));
        // Создаем и возвращаем конус
        return BRepPrimAPI_MakeCone(axis, radius, radiusEnd, length).Shape();
    }

    // Дополнительный геттер для конечного радиуса
    Standard_Real getRadiusEnd() const { return radiusEnd; }
};

/**
 * @class Slot
 * @brief Класс для создания пазов на валу
 */
class Slot {
private:
    Standard_Real width;      // Ширина паза
    Standard_Real depth;      // Глубина паза
    Standard_Real length;     // Длина паза
    Standard_Real zStart;     // Начальная координата Z паза
    Standard_Real yOffset;    // Смещение по Y от центра вала

public:
    /**
     * @brief Конструктор паза
     * @param width Ширина паза
     * @param depth Глубина паза
     * @param length Длина паза
     * @param zStart Начальная координата Z
     * @param cylinderRadius Радиус цилиндра, на котором создается паз
     */
    Slot(Standard_Real width, Standard_Real depth, Standard_Real length,
         Standard_Real zStart, Standard_Real cylinderRadius)
        : width(width), depth(depth), length(length),
        zStart(zStart), yOffset(cylinderRadius - depth) {}

    /**
     * @brief Создать форму паза
     * @return Форма паза
     */
    TopoDS_Shape create() const {
        // Радиус полуокружностей на концах паза
        Standard_Real radiusSemi = width / 2.0;

        // Создаем прямоугольную часть паза
        gp_Pnt originRect(-width / 2.0, yOffset, zStart);
        TopoDS_Shape rect = BRepPrimAPI_MakeBox(originRect, width, depth, length).Shape();

        // Создаем левую полуокружность
        gp_Ax2 axisLeft(gp_Pnt(0, yOffset, zStart), gp_Dir(0.0, 1.0, 0.0));
        TopoDS_Shape circleLeft = BRepPrimAPI_MakeCylinder(axisLeft, radiusSemi, depth).Shape();

        // Создаем правую полуокружность
        gp_Ax2 axisRight(gp_Pnt(0, yOffset, zStart + length), gp_Dir(0.0, 1.0, 0.0));
        TopoDS_Shape circleRight = BRepPrimAPI_MakeCylinder(axisRight, radiusSemi, depth).Shape();

        // Объединяем части паза
        BRepAlgoAPI_Fuse fuseSlot1(rect, circleLeft);
        if (!fuseSlot1.IsDone()) {
            throw std::runtime_error("Ошибка при объединении паза с левой окружностью");
        }

        BRepAlgoAPI_Fuse fuseSlot2(fuseSlot1, circleRight);
        if (!fuseSlot2.IsDone()) {
            throw std::runtime_error("Ошибка при объединении паза с правой окружностью");
        }

        return fuseSlot2.Shape();
    }
};

/**
 * @class ShaftBuilder
 * @brief Класс для построения полного вала
 */
class ShaftBuilder {
private:
    std::vector<std::unique_ptr<ShaftSegment>> segments;  // Сегменты вала
    std::vector<Slot> slots;                             // Пазы на валу
    Standard_Real chamferLength;                         // Длина фаски
    Standard_Real chamferAngle;                          // Угол фаски в градусах
    TopoDS_Shape finalShape;                             // Итоговая форма вала
    Standard_Real currentZCoord;                         // Текущая координата Z для добавления сегментов

public:
    /**
     * @brief Конструктор строителя вала
     * @param chamferLength Длина фаски
     * @param chamferAngle Угол фаски в градусах
     */
    ShaftBuilder(Standard_Real chamferLength = 0.025, Standard_Real chamferAngle = 45.0)
        : chamferLength(chamferLength), chamferAngle(chamferAngle), currentZCoord(0.0) {}

    /**
     * @brief Добавить цилиндрический сегмент
     * @param length Длина сегмента
     * @param diameter Диаметр сегмента
     * @param zStart Опциональная начальная координата Z (если не указана, используется текущая)
     */
    void addCylinder(Standard_Real length, Standard_Real diameter, Standard_Real zStart = -1.0) {
        // Если zStart не указан, используем текущую координату
        if (zStart < 0.0) {
            zStart = currentZCoord;
        }

        segments.push_back(std::make_unique<CylinderSegment>(zStart, length, diameter));
        std::cout << "Cylinder added (Z=" << zStart << " to " << zStart + length
                  << ", diameter=" << diameter << ")" << std::endl;

        // Обновляем текущую координату Z
        currentZCoord = zStart + length;
    }

    /**
     * @brief Добавить конический сегмент
     * @param length Длина сегмента
     * @param diameterStart Начальный диаметр
     * @param diameterEnd Конечный диаметр
     * @param zStart Опциональная начальная координата Z (если не указана, используется текущая)
     */
    void addCone(Standard_Real length, Standard_Real diameterStart,
                 Standard_Real diameterEnd, Standard_Real zStart = -1.0) {
        // Если zStart не указан, используем текущую координату
        if (zStart < 0.0) {
            zStart = currentZCoord;
        }

        segments.push_back(std::make_unique<ConeSegment>(
            zStart, length, diameterStart, diameterEnd));
        std::cout << "Cone added (Z=" << zStart << " to " << zStart + length
                  << ", diameter from " << diameterStart << " to " << diameterEnd << ")" << std::endl;

        // Обновляем текущую координату Z
        currentZCoord = zStart + length;
    }

    /**
 * @brief Уменьшить диаметр цилиндра с указанным индексом
 * @param index Индекс цилиндра в массиве сегментов
 * @param tolerance Величина уменьшения диаметра (по умолчанию 0.3 мм)
 * @return true если операция выполнена успешно, иначе false
 */
    bool reduceCylinderDiameter(size_t index, Standard_Real tolerance = 0.3) {
        if (index >= segments.size()) {
            std::cerr << "Error: Segment index " << index << " out of range" << std::endl;
            return false;
        }

        CylinderSegment* cylinder = dynamic_cast<CylinderSegment*>(segments[index].get());
        if (!cylinder) {
            std::cerr << "Error: Segment at index " << index << " is not a cylinder" << std::endl;
            return false;
        }

        // Получаем параметры текущего цилиндра
        Standard_Real zStart = cylinder->getZStart();
        Standard_Real length = cylinder->getLength();
        Standard_Real diameter = cylinder->getRadius() * 2.0; // Преобразуем радиус в диаметр

        // Создаем новый цилиндр с уменьшенным диаметром
        segments[index] = std::make_unique<CylinderSegment>(zStart, length, diameter - tolerance);

        std::cout << "Cylinder at index " << index << " reduced from diameter "
                  << diameter << " to " << (diameter - tolerance) << std::endl;

        return true;
    }

    /**
     * @brief Добавить паз
     * @param width Ширина паза
     * @param depth Глубина паза
     * @param length Длина паза
     * @param zStart Начальная координата Z
     * @param cylinderRadius Радиус цилиндра, на котором создается паз
     */
    void addSlot(Standard_Real width, Standard_Real depth, Standard_Real length,
                 Standard_Real zStart, Standard_Real cylinderRadius) {
        slots.push_back(Slot(width, depth, length, zStart, cylinderRadius));
        std::cout << "Slot added (Z=" << zStart << " to " << zStart + length
                  << ", width=" << width << ", depth=" << depth << ")" << std::endl;
    }

    /**
     * @brief Построить модель вала
     */
    void build() {
        if (segments.empty()) {
            throw std::runtime_error("Нет сегментов для построения вала");
        }

        // Создаем первый сегмент
        finalShape = segments[0]->create();

        // Добавляем остальные сегменты
        for (size_t i = 1; i < segments.size(); ++i) {
            BRepAlgoAPI_Fuse fuse(finalShape, segments[i]->create());
            if (!fuse.IsDone()) {
                throw std::runtime_error("Ошибка при объединении сегмента " + std::to_string(i));
            }
            finalShape = fuse.Shape();
        }

        std::cout << "Segments fused successfully" << std::endl;

        // Создаем фаски на краях вала
        addChamfers();

        // Вырезаем пазы
        cutSlots();
    }

    /**
     * @brief Экспортировать модель в файл STEP
     * @param filename Имя файла
     * @return true если экспорт прошел успешно, иначе false
     */
    bool exportToSTEP(const std::string& filename) const {
        STEPControl_Writer writer;

        if (writer.Transfer(finalShape, STEPControl_AsIs) != IFSelect_RetDone) {
            std::cout << "STEP transfer failed" << std::endl;
            return false;
        }

        if (!writer.Write(filename.c_str())) {
            std::cout << "STEP write failed" << std::endl;
            return false;
        }

        std::cout << "Shaft exported to " << filename << std::endl;
        return true;
    }

private:
    /**
     * @brief Добавить фаски на краях вала
     */
    void addChamfers() {
        // Получаем крайние Z-координаты
        Standard_Real zMin = segments.front()->getZStart();
        Standard_Real zMax = segments.back()->getZEnd();

        BRepFilletAPI_MakeChamfer chamferMaker(finalShape);

        // Расчет размера фаски в зависимости от угла
        Standard_Real chamferDist = chamferLength * tan(chamferAngle * M_PI / 180.0);

        // Находим края для фасок
        TopoDS_Edge leftEdge, rightEdge;
        TopTools_IndexedMapOfShape edgeMap;
        TopExp::MapShapes(finalShape, TopAbs_EDGE, edgeMap);
        std::cout << "Total edges found: " << edgeMap.Extent() << std::endl;

        for (Standard_Integer i = 1; i <= edgeMap.Extent(); ++i) {
            TopoDS_Edge edge = TopoDS::Edge(edgeMap(i));
            TopExp_Explorer vertexExp(edge, TopAbs_VERTEX);

            while (vertexExp.More()) {
                TopoDS_Vertex vertex = TopoDS::Vertex(vertexExp.Current());
                gp_Pnt pnt = BRep_Tool::Pnt(vertex);

                // Проверяем, находится ли вершина на крайних плоскостях
                if (fabs(pnt.Z() - zMin) < 1e-6 && leftEdge.IsNull()) {
                    leftEdge = edge;
                    std::cout << "Left edge found at Z=" << zMin << std::endl;
                }

                if (fabs(pnt.Z() - zMax) < 1e-6 && rightEdge.IsNull()) {
                    rightEdge = edge;
                    std::cout << "Right edge found at Z=" << zMax << std::endl;
                }

                vertexExp.Next();
            }
        }

        // Добавляем фаску на левом крае
        if (!leftEdge.IsNull()) {
            try {
                // Создаем фаску с двумя размерами для угла 45 градусов
                chamferMaker.Add(chamferDist, chamferDist, leftEdge,
                                 TopoDS::Face(TopExp_Explorer(finalShape, TopAbs_FACE).Current()));
                std::cout << "Chamfer added to left edge successfully" << std::endl;
            } catch (const Standard_Failure& e) {
                std::cout << "Error adding chamfer to left edge: "
                          << e.GetMessageString() << std::endl;
            }
        } else {
            std::cout << "Warning: Left edge not found" << std::endl;
        }

        // Добавляем фаску на правом крае
        if (!rightEdge.IsNull()) {
            try {
                // Создаем фаску с двумя размерами для угла 45 градусов
                chamferMaker.Add(chamferDist, chamferDist, rightEdge,
                                 TopoDS::Face(TopExp_Explorer(finalShape, TopAbs_FACE).Current()));
                std::cout << "Chamfer added to right edge successfully" << std::endl;
            } catch (const Standard_Failure& e) {
                std::cout << "Error adding chamfer to right edge: "
                          << e.GetMessageString() << std::endl;
            }
        } else {
            std::cout << "Warning: Right edge not found" << std::endl;
        }

        finalShape = chamferMaker.Shape();
        std::cout << "Chamfers applied, preparing to cut slots" << std::endl;
    }

    /**
     * @brief Вырезать пазы из вала
     */
    void cutSlots() {
        for (size_t i = 0; i < slots.size(); ++i) {
            try {
                TopoDS_Shape slotShape = slots[i].create();
                BRepAlgoAPI_Cut cut(finalShape, slotShape);

                if (!cut.IsDone()) {
                    throw std::runtime_error("Ошибка при вырезании паза " + std::to_string(i));
                }

                finalShape = cut.Shape();
                std::cout << "Slot " << i << " cut applied" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error cutting slot " << i << ": " << e.what() << std::endl;
            }
        }
    }
};
#endif // SHAFT_BUILDER_H
