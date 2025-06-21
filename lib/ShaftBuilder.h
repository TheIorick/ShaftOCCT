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
#include <TopTools_IndexedMapOfShape.hxx> // Коллекция топологических объектов
#include <TopExp.hxx>                    // Топологические операции
#include <TopoDS.hxx>                    // Приведение типов для топологических объектов
#include <BRep_Tool.hxx>                 // Инструменты для работы с геометрией
#include <BRepBndLib.hxx>                // Работа с ограничивающими боксами
#include <Bnd_Box.hxx>                   // Ограничивающий бокс
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <Standard_DefineAlloc.hxx>

#include "Slot.h"  // Подключаем класс Slot
#include "ShaftProportions.h"

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
    ShaftSegment(Standard_Real zStart, Standard_Real length, Standard_Real radius)
        : zStart(zStart), length(length), radius(radius) {}

    virtual ~ShaftSegment() = default;

    virtual TopoDS_Shape create() const = 0;

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
    CylinderSegment(Standard_Real zStart, Standard_Real length, Standard_Real diameter)
        : ShaftSegment(zStart, length, diameter / 2.0) {}

    TopoDS_Shape create() const override {
        gp_Ax2 axis(gp_Pnt(0.0, 0.0, zStart), gp_Dir(0.0, 0.0, 1.0));
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
    ConeSegment(Standard_Real zStart, Standard_Real length,
                Standard_Real diameterStart, Standard_Real diameterEnd)
        : ShaftSegment(zStart, length, diameterStart / 2.0),
        radiusEnd(diameterEnd / 2.0) {}

    TopoDS_Shape create() const override {
        gp_Ax2 axis(gp_Pnt(0.0, 0.0, zStart), gp_Dir(0.0, 0.0, 1.0));
        return BRepPrimAPI_MakeCone(axis, radius, radiusEnd, length).Shape();
    }

    Standard_Real getRadiusEnd() const { return radiusEnd; }
};

/**
 * @class ShaftBuilder
 * @brief Класс для построения полного вала
 */
class ShaftBuilder {
private:
    std::vector<Slot> m_slots;                             // Пазы на валу
    std::vector<std::unique_ptr<ShaftSegment>> segments; // Сегменты вала
    Standard_Real chamferLength;                         // Длина фаски
    Standard_Real chamferAngle;                          // Угол фаски в градусах
    TopoDS_Shape finalShape;                             // Итоговая форма вала
    Standard_Real currentZCoord;                         // Текущая координата Z для добавления сегментов

public:
    ShaftBuilder(Standard_Real chamferLength = 0.025, Standard_Real chamferAngle = 45.0)
        : chamferLength(chamferLength), chamferAngle(chamferAngle), currentZCoord(0.0) {}

    void addCylinder(Standard_Real length, Standard_Real diameter, Standard_Real zStart = -1.0) {
        if (zStart < 0.0) zStart = currentZCoord;
        segments.push_back(std::make_unique<CylinderSegment>(zStart, length, diameter));
        std::cout << "Cylinder added (Z=" << zStart << " to " << zStart + length
                  << ", diameter=" << diameter << ")" << std::endl;
        currentZCoord = zStart + length;
    }

    void addCone(Standard_Real length, Standard_Real diameterStart,
                 Standard_Real diameterEnd, Standard_Real zStart = -1.0) {
        if (zStart < 0.0) zStart = currentZCoord;
        segments.push_back(std::make_unique<ConeSegment>(zStart, length, diameterStart, diameterEnd));
        std::cout << "Cone added (Z=" << zStart << " to " << zStart + length
                  << ", diameter from " << diameterStart << " to " << diameterEnd << ")" << std::endl;
        currentZCoord = zStart + length;
    }

    const TopoDS_Shape& getFinalShape() const { return finalShape; }

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
        Standard_Real zStart = cylinder->getZStart();
        Standard_Real length = cylinder->getLength();
        Standard_Real diameter = cylinder->getRadius() * 2.0;
        segments[index] = std::make_unique<CylinderSegment>(zStart, length, diameter - tolerance);
        std::cout << "Cylinder at index " << index << " reduced from diameter "
                  << diameter << " to " << (diameter - tolerance) << std::endl;
        return true;
    }

    void addSlot(Standard_Real width, Standard_Real depth, Standard_Real length,
                 Standard_Real zStart, Standard_Real cylinderRadius) {
        m_slots.push_back(Slot(width, depth, length, zStart, cylinderRadius));
        std::cout << "Slot added (Z=" << zStart << " to " << zStart + length
                  << ", width=" << width << ", depth=" << depth << ")" << std::endl;
    }

    void build() {
        if (segments.empty()) throw std::runtime_error("No segments to build the shaft");
        finalShape = segments[0]->create();
        for (size_t i = 1; i < segments.size(); ++i) {
            BRepAlgoAPI_Fuse fuse(finalShape, segments[i]->create());
            if (!fuse.IsDone()) throw std::runtime_error("Error fusing segment " + std::to_string(i));
            finalShape = fuse.Shape();
        }
        std::cout << "Segments fused successfully" << std::endl;
        addChamfers();
        cutSlots();
    }

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

    void buildFromProportions(const ShaftProportions& proportions) {
        currentZCoord = 0.0;
        segments.clear();
        m_slots.clear();
        size_t segmentCount = proportions.getSegmentCount();
        double chamferLength = proportions.getChamferLength();
        this->chamferLength = chamferLength;
        std::cout << "Building shaft from " << segmentCount << " segments" << std::endl;
        std::cout << "Chamfer length: " << chamferLength << " mm" << std::endl;
        for (size_t i = 0; i < segmentCount; ++i) {
            auto segmentInfo = proportions.getSegmentInfo(i);
            std::string type = std::get<0>(segmentInfo);
            double length = std::get<1>(segmentInfo);
            double diameter = std::get<2>(segmentInfo);
            double diameterEnd = std::get<3>(segmentInfo);
            bool needsReduction = std::get<4>(segmentInfo);

            if (i == 0 || i == segmentCount - 1) length += chamferLength;
            std::cout << "Adding segment " << i << " (" << proportions.getSegmentName(i)
                      << "): type=" << type << ", length=" << length << ", diameter=" << diameter;
            if (type == "cylinder") {
                addCylinder(length, diameter);
                if (needsReduction) {
                    std::cout << " (reduced by 0.3 mm)" << std::endl;
                    reduceCylinderDiameter(segments.size() - 1);
                } else std::cout << std::endl;
            } else if (type == "cone") {
                std::cout << ", end diameter=" << diameterEnd << std::endl;
                addCone(length, diameter, diameterEnd);
            }
        }
        size_t slotCount = proportions.getSlotCount();
        for (size_t i = 0; i < slotCount; ++i) {
            auto slotInfo = proportions.getSlotInfo(i);
            double width = std::get<0>(slotInfo);
            double depth = std::get<1>(slotInfo);
            double length = std::get<2>(slotInfo);
            double offset = std::get<3>(slotInfo); // This is the offset from segment start
            size_t segmentIndex = std::get<4>(slotInfo);

            double segmentDiameter = proportions.getSegmentDiameter(segmentIndex);

            // Calculate the actual zStart for the slot
            Standard_Real segmentZStart = segments[segmentIndex]->getZStart();
            Standard_Real segmentLength = segments[segmentIndex]->getLength();
            Standard_Real segmentZEnd = segmentZStart + segmentLength;

            Standard_Real slotZStart = segmentZStart + offset;

            // Убедиться, что паз не выходит за пределы сегмента
            Standard_Real slotZEnd = slotZStart + length;
            if (slotZEnd > segmentZEnd) {
                length = segmentZEnd - slotZStart; // Укорачиваем паз
                if (length < 0) length = 0; // Если паз полностью выходит за пределы, его длина становится 0
                std::cout << "Slot length adjusted to " << length << " due to segment boundary." << std::endl;
            }

            std::cout << "Adding slot " << i << " on segment " << segmentIndex
                      << ": width=" << width << ", depth=" << depth
                      << ", length=" << length << ", position (offset from segment start)=" << offset
                      << ", segment actual zStart=" << segmentZStart
                      << ", slot actual zStart=" << slotZStart
                      << ", cylinder diameter=" << segmentDiameter << std::endl;
            addSlot(width, depth, length, slotZStart, segmentDiameter / 2.0);
        }
    }

private:
    void addChamfers() {
        Standard_Real zMin = segments.front()->getZStart();
        Standard_Real zMax = segments.back()->getZEnd();
        BRepFilletAPI_MakeChamfer chamferMaker(finalShape);
        Standard_Real chamferDist = chamferLength * tan(chamferAngle * M_PI / 180.0);
        TopTools_IndexedMapOfShape edgeMap;
        TopExp::MapShapes(finalShape, TopAbs_EDGE, edgeMap);
        std::cout << "Total edges found: " << edgeMap.Extent() << std::endl;
        TopoDS_Edge leftEdge, rightEdge;
        for (Standard_Integer i = 1; i <= edgeMap.Extent(); ++i) {
            TopoDS_Edge edge = TopoDS::Edge(edgeMap(i));
            TopExp_Explorer vertexExp(edge, TopAbs_VERTEX);
            while (vertexExp.More()) {
                TopoDS_Vertex vertex = TopoDS::Vertex(vertexExp.Current());
                gp_Pnt pnt = BRep_Tool::Pnt(vertex);
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
        if (!leftEdge.IsNull()) {
            try {
                chamferMaker.Add(chamferDist, chamferDist, leftEdge,
                                 TopoDS::Face(TopExp_Explorer(finalShape, TopAbs_FACE).Current()));
                std::cout << "Chamfer added to left edge successfully" << std::endl;
            } catch (const Standard_Failure& e) {
                std::cout << "Error adding chamfer to left edge: " << e.GetMessageString() << std::endl;
            }
        } else std::cout << "Warning: Left edge not found" << std::endl;
        if (!rightEdge.IsNull()) {
            try {
                chamferMaker.Add(chamferDist, chamferDist, rightEdge,
                                 TopoDS::Face(TopExp_Explorer(finalShape, TopAbs_FACE).Current()));
                std::cout << "Chamfer added to right edge successfully" << std::endl;
            } catch (const Standard_Failure& e) {
                std::cout << "Error adding chamfer to right edge: " << e.GetMessageString() << std::endl;
            }
        } else std::cout << "Warning: Right edge not found" << std::endl;
        finalShape = chamferMaker.Shape();
        std::cout << "Chamfers applied, preparing to cut slots" << std::endl;
    }

    void cutSlots() {
        for (size_t i = 0; i < m_slots.size(); ++i) {
            try {
                // Создаем форму паза
                TopoDS_Shape slotShape = m_slots[i].create();
                // Вырезаем паз из вала
                BRepAlgoAPI_Cut cut(finalShape, slotShape);
                if (!cut.IsDone()) {
                    throw std::runtime_error("Error cutting slot " + std::to_string(i));
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
