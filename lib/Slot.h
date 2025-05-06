#ifndef SLOT_H
#define SLOT_H

#include <string>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <stdexcept>
#include <iostream>


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
    Slot(Standard_Real width, Standard_Real depth, Standard_Real length,
         Standard_Real zStart, Standard_Real cylinderRadius)
        : width(width), depth(depth), length(length), zStart(zStart) {
        if (depth > cylinderRadius) {
            throw std::invalid_argument("Глубина паза (" + std::to_string(depth) +
                                        ") превышает радиус цилиндра (" + std::to_string(cylinderRadius) + ")");
        }
        yOffset = cylinderRadius - depth;
        std::cout << "Паз создан: радиус цилиндра=" << cylinderRadius << ", yOffset=" << yOffset << std::endl;
    }

    TopoDS_Shape create() const {
        Standard_Real radiusSemi = width / 2.0;

        // Создаем прямоугольную часть паза
        gp_Pnt originRect(-width / 2.0, yOffset, zStart);
        TopoDS_Shape rect = BRepPrimAPI_MakeBox(originRect, width, depth, length).Shape();

        // Создаем левую полуокружность
        gp_Ax2 axisLeft(gp_Pnt(0.0, yOffset, zStart), gp_Dir(0.0, 1.0, 0.0));
        TopoDS_Shape circleLeft = BRepPrimAPI_MakeCylinder(axisLeft, radiusSemi, depth).Shape();

        // Создаем правую полуокружность
        gp_Ax2 axisRight(gp_Pnt(0.0, yOffset, zStart + length), gp_Dir(0.0, 1.0, 0.0));
        TopoDS_Shape circleRight = BRepPrimAPI_MakeCylinder(axisRight, radiusSemi, depth).Shape();

        // Объединяем части паза
        BRepAlgoAPI_Fuse fuse1(rect, circleLeft);
        if (!fuse1.IsDone()) {
            throw std::runtime_error("Error fusing slot with left circle");
        }

        BRepAlgoAPI_Fuse fuse2(fuse1.Shape(), circleRight);
        if (!fuse2.IsDone()) {
            throw std::runtime_error("Error fusing slot with right circle");
        }

        return fuse2.Shape();
    }
};

#endif // SLOT_H
