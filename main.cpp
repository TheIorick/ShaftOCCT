#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <iostream>

int main() {
    std::cout << "Starting shaft construction..." << std::endl;

    gp_Ax2 axis(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, 0.0, 1.0));

    Standard_Real chamferLength = 0.025;
    Standard_Real radius1 = 23.0 / 2.0;
    Standard_Real length1 = 18.0 + chamferLength;
    TopoDS_Shape cylinder1 = BRepPrimAPI_MakeCylinder(axis, radius1, length1).Shape();
    std::cout << "Cylinder 1 created (Z=0 to " << length1 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1));
    Standard_Real radius2 = 25.0 / 2.0;
    Standard_Real length2 = 15.0;
    TopoDS_Shape cylinder2 = BRepPrimAPI_MakeCylinder(axis, radius2, length2).Shape();
    std::cout << "Cylinder 2 created (Z=" << length1 << " to " << length1 + length2 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2));
    Standard_Real radius3 = 23.0 / 2.0;
    Standard_Real length3 = 3.0;
    TopoDS_Shape cylinder3 = BRepPrimAPI_MakeCylinder(axis, radius3, length3).Shape();
    std::cout << "Cylinder 3 created (Z=" << length1 + length2 << " to " << length1 + length2 + length3 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3));
    Standard_Real radius4 = 23.0 / 2.0;
    Standard_Real length4 = 27.0;
    TopoDS_Shape cylinder4 = BRepPrimAPI_MakeCylinder(axis, radius4, length4).Shape();
    std::cout << "Cylinder 4 created (Z=" << length1 + length2 + length3 << " to " << length1 + length2 + length3 + length4 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4));
    Standard_Real radius5 = 23.0 / 2.0;
    Standard_Real length5 = 3.0;
    TopoDS_Shape cylinder5 = BRepPrimAPI_MakeCylinder(axis, radius5, length5).Shape();
    std::cout << "Cylinder 5 created (Z=" << length1 + length2 + length3 + length4 << " to " << length1 + length2 + length3 + length4 + length5 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5));
    Standard_Real radius6 = 35.0 / 2.0;
    Standard_Real length6 = 14.0;
    TopoDS_Shape cylinder6 = BRepPrimAPI_MakeCylinder(axis, radius6, length6).Shape();
    std::cout << "Cylinder 6 created (Z=" << length1 + length2 + length3 + length4 + length5 << " to " << length1 + length2 + length3 + length4 + length5 + length6 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6));
    Standard_Real radius7_start = 35.0 / 2.0;
    Standard_Real radius7_end = 40.0 / 2.0;
    Standard_Real length7 = 5.0;
    TopoDS_Shape cone7 = BRepPrimAPI_MakeCone(axis, radius7_start, radius7_end, length7).Shape();
    std::cout << "Cone 7 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7));
    Standard_Real radius8 = 40.0 / 2.0;
    Standard_Real length8 = 40.0;
    TopoDS_Shape cylinder8 = BRepPrimAPI_MakeCylinder(axis, radius8, length8).Shape();
    std::cout << "Cylinder 8 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8));
    Standard_Real radius9 = 23.0 / 2.0;
    Standard_Real length9 = 3.0;
    TopoDS_Shape cylinder9 = BRepPrimAPI_MakeCylinder(axis, radius9, length9).Shape();
    std::cout << "Cylinder 9 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9));
    Standard_Real radius10 = 27.0 / 2.0;
    Standard_Real length10 = 59.0;
    TopoDS_Shape cylinder10 = BRepPrimAPI_MakeCylinder(axis, radius10, length10).Shape();
    std::cout << "Cylinder 10 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10));
    Standard_Real radius11 = 23.0 / 2.0;
    Standard_Real length11 = 3.0;
    TopoDS_Shape cylinder11 = BRepPrimAPI_MakeCylinder(axis, radius11, length11).Shape();
    std::cout << "Cylinder 11 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11));
    Standard_Real radius12 = 25.0 / 2.0;
    Standard_Real length12 = 21.0;
    TopoDS_Shape cylinder12 = BRepPrimAPI_MakeCylinder(axis, radius12, length12).Shape();
    std::cout << "Cylinder 12 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12 << ")" << std::endl;

    axis.SetLocation(gp_Pnt(0.0, 0.0, length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12));
    Standard_Real radius13 = 23.0 / 2.0;
    Standard_Real length13 = 19.0;
    TopoDS_Shape cylinder13 = BRepPrimAPI_MakeCylinder(axis, radius13, length13 + chamferLength).Shape();
    std::cout << "Cylinder 13 created (Z=" << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12 << " to " << length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12 + length13 + chamferLength << ")" << std::endl;

    BRepAlgoAPI_Fuse fuse1(cylinder1, cylinder2);
    if (!fuse1.IsDone()) { std::cout << "Fuse 1 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse2(fuse1, cylinder3);
    if (!fuse2.IsDone()) { std::cout << "Fuse 2 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse3(fuse2, cylinder4);
    if (!fuse3.IsDone()) { std::cout << "Fuse 3 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse4(fuse3, cylinder5);
    if (!fuse4.IsDone()) { std::cout << "Fuse 4 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse5(fuse4, cylinder6);
    if (!fuse5.IsDone()) { std::cout << "Fuse 5 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse6(fuse5, cone7);
    if (!fuse6.IsDone()) { std::cout << "Fuse 6 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse7(fuse6, cylinder8);
    if (!fuse7.IsDone()) { std::cout << "Fuse 7 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse8(fuse7, cylinder9);
    if (!fuse8.IsDone()) { std::cout << "Fuse 8 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse9(fuse8, cylinder10);
    if (!fuse9.IsDone()) { std::cout << "Fuse 9 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse10(fuse9, cylinder11);
    if (!fuse10.IsDone()) { std::cout << "Fuse 10 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse11(fuse10, cylinder12);
    if (!fuse11.IsDone()) { std::cout << "Fuse 11 failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse12(fuse11, cylinder13);
    if (!fuse12.IsDone()) { std::cout << "Fuse 12 failed" << std::endl; return 1; }
    TopoDS_Shape shaft = fuse12.Shape();
    std::cout << "Shaft fused successfully" << std::endl;

    BRepFilletAPI_MakeChamfer chamferMaker(shaft);
    Standard_Real chamferDist = 0.025;

    TopoDS_Edge leftEdge, rightEdge;
    TopTools_IndexedMapOfShape edgeMap;
    TopExp::MapShapes(shaft, TopAbs_EDGE, edgeMap);
    std::cout << "Total edges found: " << edgeMap.Extent() << std::endl;

    for (Standard_Integer i = 1; i <= edgeMap.Extent(); ++i) {
        TopoDS_Edge edge = TopoDS::Edge(edgeMap(i));
        TopExp_Explorer vertexExp(edge, TopAbs_VERTEX);
        while (vertexExp.More()) {
            TopoDS_Vertex vertex = TopoDS::Vertex(vertexExp.Current());
            gp_Pnt pnt = BRep_Tool::Pnt(vertex);
            std::cout << "Vertex Z: " << pnt.Z() << std::endl;
            if (fabs(pnt.Z() - 0.0) < 1e-6 && leftEdge.IsNull()) {
                leftEdge = edge;
                std::cout << "Left edge found at Z=0" << std::endl;
            }
            if (fabs(pnt.Z() - 230.05) < 1e-6 && rightEdge.IsNull()) {
                rightEdge = edge;
                std::cout << "Right edge found at Z=230.05" << std::endl;
            }
            vertexExp.Next();
        }
    }

    if (!leftEdge.IsNull()) {
        std::cout << "Adding chamfer to left edge" << std::endl;
        try {
            chamferMaker.Add(chamferDist, leftEdge);
            std::cout << "Chamfer added to left edge successfully" << std::endl;
        } catch (const Standard_Failure& e) {
            std::cout << "Error adding chamfer to left edge: " << e.GetMessageString() << std::endl;
            return 1;
        }
    } else {
        std::cout << "Warning: Left edge not found" << std::endl;
    }

    if (!rightEdge.IsNull()) {
        std::cout << "Adding chamfer to right edge" << std::endl;
        try {
            chamferMaker.Add(chamferDist, rightEdge);
            std::cout << "Chamfer added to right edge successfully" << std::endl;
        } catch (const Standard_Failure& e) {
            std::cout << "Error adding chamfer to right edge: " << e.GetMessageString() << std::endl;
            return 1;
        }
    } else {
        std::cout << "Warning: Right edge not found" << std::endl;
    }

    shaft = chamferMaker.Shape();
    std::cout << "Chamfers applied, generating final shape" << std::endl;

    // Паз на 4-м цилиндре: длина 10 мм, начало на Z=44.5 мм
    Standard_Real width4 = 8.0;
    Standard_Real depth4 = 5.0;
    Standard_Real length4_paz = 10.0;
    Standard_Real startZ4 = 44.5;
    Standard_Real yOffset4 = radius4 - depth4;

    // Радиус полуокружностей равен половине ширины паза
    Standard_Real radius4_semi = width4 / 2.0; // 4 мм
    // Длина прямоугольной части
    Standard_Real rect_length4 = length4_paz;

    // Прямоугольная часть паза
    gp_Pnt origin4_rect(-width4 / 2.0, yOffset4, startZ4);
    TopoDS_Shape rect4 = BRepPrimAPI_MakeBox(origin4_rect, width4, depth4, length4_paz).Shape();

    // Целая окружность слева
    gp_Ax2 axis4_left(gp_Pnt(0, yOffset4, startZ4), gp_Dir(0.0, 1.0, 0.0));
    TopoDS_Shape circle4_left = BRepPrimAPI_MakeCylinder(axis4_left, radius4_semi, depth4).Shape(); // Убрали M_PI для полной окружности

    // Целая окружность справа
    gp_Ax2 axis4_right(gp_Pnt(0, yOffset4, startZ4 + length4_paz), gp_Dir(0.0, 1.0, 0.0));
    TopoDS_Shape circle4_right = BRepPrimAPI_MakeCylinder(axis4_right, radius4_semi, depth4).Shape(); // Убрали M_PI для полной окружности

    // Объединяем части паза
    BRepAlgoAPI_Fuse fuse_slot4_1(rect4, circle4_left);
    if (!fuse_slot4_1.IsDone()) { std::cout << "Fuse slot 4 (left circle) failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse_slot4_2(fuse_slot4_1, circle4_right);
    if (!fuse_slot4_2.IsDone()) { std::cout << "Fuse slot 4 (right circle) failed" << std::endl; return 1; }
    TopoDS_Shape paz4 = fuse_slot4_2.Shape();
    std::cout << "Slot 4 created" << std::endl;

    BRepAlgoAPI_Cut cutPaz4(shaft, paz4);
    if (!cutPaz4.IsDone()) { std::cout << "Cut 4 failed" << std::endl; return 1; }
    shaft = cutPaz4.Shape();
    std::cout << "Slot 4 cut applied" << std::endl;

    // Паз на 10-м цилиндре: длина 22 мм, начало на Z=133 мм
    Standard_Real width10 = 8.0;
    Standard_Real depth10 = 4.0;
    Standard_Real length10_paz = 22.0;
    Standard_Real startZ10 = 133.0;
    Standard_Real yOffset10 = radius10 - depth10;

    // Радиус полуокружностей
    Standard_Real radius10_semi = width10 / 2.0; // 4 мм
    // Длина прямоугольной части
    Standard_Real rect_length10 = length10_paz;

    // Прямоугольная часть паза
    gp_Pnt origin10_rect(-width10 / 2.0, yOffset10, startZ10 + radius10_semi);
    TopoDS_Shape rect10 = BRepPrimAPI_MakeBox(origin10_rect, width10, depth10, rect_length10).Shape();

    // Полуокружность слева
    gp_Ax2 axis10_left(gp_Pnt(0, yOffset10, startZ10 + radius10_semi), gp_Dir(0.0, 1.0, 0.0));
    TopoDS_Shape semicircle10_left = BRepPrimAPI_MakeCylinder(axis10_left, radius10_semi, depth10).Shape();

    // Полуокружность справа
    gp_Ax2 axis10_right(gp_Pnt(0, yOffset10, startZ10 + radius10_semi + rect_length10), gp_Dir(0.0, 1.0, 0.0));
    TopoDS_Shape semicircle10_right = BRepPrimAPI_MakeCylinder(axis10_right, radius10_semi, depth10).Shape();

    // Объединяем части паза
    BRepAlgoAPI_Fuse fuse_slot10_1(rect10, semicircle10_left);
    if (!fuse_slot10_1.IsDone()) { std::cout << "Fuse slot 10 (left semicircle) failed" << std::endl; return 1; }
    BRepAlgoAPI_Fuse fuse_slot10_2(fuse_slot10_1, semicircle10_right);
    if (!fuse_slot10_2.IsDone()) { std::cout << "Fuse slot 10 (right semicircle) failed" << std::endl; return 1; }
    TopoDS_Shape paz10 = fuse_slot10_2.Shape();
    std::cout << "Slot 10 created" << std::endl;

    BRepAlgoAPI_Cut cutPaz10(shaft, paz10);
    if (!cutPaz10.IsDone()) { std::cout << "Cut 10 failed" << std::endl; return 1; }
    shaft = cutPaz10.Shape();
    std::cout << "Slot 10 cut applied" << std::endl;

    STEPControl_Writer writer;
    if (writer.Transfer(shaft, STEPControl_AsIs) != IFSelect_RetDone) {
        std::cout << "STEP transfer failed" << std::endl;
        return 1;
    }
    if (!writer.Write("shaft.step")) {
        std::cout << "STEP write failed" << std::endl;
        return 1;
    }
    std::cout << "Shaft exported to shaft.step" << std::endl;

    return 0;
}
