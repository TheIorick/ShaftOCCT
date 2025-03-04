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

int main() {
    // Определяем ось Z для всех цилиндров
    gp_Ax2 axis(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(0.0, 0.0, 1.0));

    // Фаска на левом конце (Z=0): от Ø23 мм до Ø(23-2*0.025)=22.95 мм, длина 0.025 мм
    Standard_Real chamferLength = 0.025; // Длина фаски
    Standard_Real chamferStartRadius = 23.0 / 2.0; // Начальный радиус (Ø23 мм)
    Standard_Real chamferEndRadius = (23.0 - 2 * 0.025) / 2.0; // Конечный радиус (Ø22.95 мм)
    TopoDS_Shape leftChamfer = BRepPrimAPI_MakeCone(axis, chamferStartRadius, chamferEndRadius, chamferLength).Shape();

    // Сдвигаем ось для первого цилиндра после фаски
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength));

    // 1-й участок: Ø23 мм, длина 18 мм (начинается с Z=0.025 мм)
    Standard_Real radius1 = 23.0 / 2.0;
    Standard_Real length1 = 18.0;
    TopoDS_Shape cylinder1 = BRepPrimAPI_MakeCylinder(axis, radius1, length1).Shape();

    // 2-й участок: Ø25 мм, длина 15 мм (начинается с Z=0.025+18=18.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1));
    Standard_Real radius2 = 25.0 / 2.0;
    Standard_Real length2 = 15.0;
    TopoDS_Shape cylinder2 = BRepPrimAPI_MakeCylinder(axis, radius2, length2).Shape();

    // 3-й участок: Ø23 мм, длина 3 мм (начинается с Z=18.025+15=33.025 мм, сечение А)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2));
    Standard_Real radius3 = 23.0 / 2.0;
    Standard_Real length3 = 3.0;
    TopoDS_Shape cylinder3 = BRepPrimAPI_MakeCylinder(axis, radius3, length3).Shape();

    // 4-й участок: Ø23 мм, длина 27 мм (начинается с Z=33.025+3=36.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3));
    Standard_Real radius4 = 23.0 / 2.0;
    Standard_Real length4 = 27.0;
    TopoDS_Shape cylinder4 = BRepPrimAPI_MakeCylinder(axis, radius4, length4).Shape();

    // 5-й участок: Ø23 мм, длина 3 мм (начинается с Z=36.025+27=63.025 мм, сечение Б)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4));
    Standard_Real radius5 = 23.0 / 2.0;
    Standard_Real length5 = 3.0;
    TopoDS_Shape cylinder5 = BRepPrimAPI_MakeCylinder(axis, radius5, length5).Shape();

    // 6-й участок: Ø35 мм, длина 14 мм (начинается с Z=63.025+3=66.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5));
    Standard_Real radius6 = 35.0 / 2.0;
    Standard_Real length6 = 14.0;
    TopoDS_Shape cylinder6 = BRepPrimAPI_MakeCylinder(axis, radius6, length6).Shape();

    // 7-й участок: переход Ø35→Ø40 мм, длина 5 мм (начинается с Z=66.025+14=80.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6));
    Standard_Real radius7_start = 35.0 / 2.0;
    Standard_Real radius7_end = 40.0 / 2.0;
    Standard_Real length7 = 5.0;
    TopoDS_Shape cone7 = BRepPrimAPI_MakeCone(axis, radius7_start, radius7_end, length7).Shape();

    // 8-й участок: Ø40 мм, длина 40 мм (начинается с Z=80.025+5=85.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7));
    Standard_Real radius8 = 40.0 / 2.0;
    Standard_Real length8 = 40.0;
    TopoDS_Shape cylinder8 = BRepPrimAPI_MakeCylinder(axis, radius8, length8).Shape();

    // 9-й участок: Ø23 мм, длина 3 мм (начинается с Z=85.025+40=125.025 мм, сечение В)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8));
    Standard_Real radius9 = 23.0 / 2.0;
    Standard_Real length9 = 3.0;
    TopoDS_Shape cylinder9 = BRepPrimAPI_MakeCylinder(axis, radius9, length9).Shape();

    // 10-й участок: Ø27 мм, длина 59 мм (начинается с Z=125.025+3=128.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9));
    Standard_Real radius10 = 27.0 / 2.0;
    Standard_Real length10 = 59.0;
    TopoDS_Shape cylinder10 = BRepPrimAPI_MakeCylinder(axis, radius10, length10).Shape();

    // 11-й участок: Ø23 мм, длина 3 мм (начинается с Z=128.025+59=187.025 мм, сечение Г)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10));
    Standard_Real radius11 = 23.0 / 2.0;
    Standard_Real length11 = 3.0;
    TopoDS_Shape cylinder11 = BRepPrimAPI_MakeCylinder(axis, radius11, length11).Shape();

    // 12-й участок: Ø25 мм, длина 21 мм (начинается с Z=187.025+3=190.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11));
    Standard_Real radius12 = 25.0 / 2.0;
    Standard_Real length12 = 21.0;
    TopoDS_Shape cylinder12 = BRepPrimAPI_MakeCylinder(axis, radius12, length12).Shape();

    // 13-й участок: Ø23 мм, длина 19 мм (начинается с Z=190.025+21=211.025 мм)
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12));
    Standard_Real radius13 = 23.0 / 2.0;
    Standard_Real length13 = 19.0 - chamferLength; // Уменьшаем длину последнего цилиндра, чтобы учесть фаску (общая длина должна быть 230 мм)
    TopoDS_Shape cylinder13 = BRepPrimAPI_MakeCylinder(axis, radius13, length13).Shape();

    // Фаска на правом конце (начинается с Z=229.975 мм): от Ø23 мм до Ø22.95 мм, длина 0.025 мм
    axis.SetLocation(gp_Pnt(0.0, 0.0, chamferLength + length1 + length2 + length3 + length4 + length5 + length6 + length7 + length8 + length9 + length10 + length11 + length12));
    TopoDS_Shape rightChamfer = BRepPrimAPI_MakeCone(axis, chamferEndRadius, chamferStartRadius, chamferLength).Shape(); // Обратный порядок радиусов для фаски

    // Объединяем все цилиндры, конус и фаски в единое тело
    BRepAlgoAPI_Fuse fuse1(leftChamfer, cylinder1);
    BRepAlgoAPI_Fuse fuse2(fuse1, cylinder2);
    BRepAlgoAPI_Fuse fuse3(fuse2, cylinder3);
    BRepAlgoAPI_Fuse fuse4(fuse3, cylinder4);
    BRepAlgoAPI_Fuse fuse5(fuse4, cylinder5);
    BRepAlgoAPI_Fuse fuse6(fuse5, cylinder6);
    BRepAlgoAPI_Fuse fuse7(fuse6, cone7);
    BRepAlgoAPI_Fuse fuse8(fuse7, cylinder8);
    BRepAlgoAPI_Fuse fuse9(fuse8, cylinder9);
    BRepAlgoAPI_Fuse fuse10(fuse9, cylinder10);
    BRepAlgoAPI_Fuse fuse11(fuse10, cylinder11);
    BRepAlgoAPI_Fuse fuse12(fuse11, cylinder12);
    BRepAlgoAPI_Fuse fuse13(fuse12, cylinder13);
    BRepAlgoAPI_Fuse fuse14(fuse13, rightChamfer);

    TopoDS_Shape shaft = fuse14.Shape();

    // Паз на 4-м цилиндре: длина 10 мм, начало на Z=44.5 мм
    Standard_Real width4 = 3.0; // Ширина паза
    Standard_Real depth4 = 3.0; // Глубина паза (11.5 мм - 3 мм = 8.5 мм)
    Standard_Real length4_paz = 10.0; // Длина паза вдоль оси Z
    Standard_Real startZ4 = 44.5; // Начало паза по Z (44.5 мм от левого края)
    Standard_Real yOffset4 = radius4 - depth4; // Смещение по Y (11.5 мм - 3 мм = 8.5 мм)
    gp_Pnt origin4(-width4 / 2.0, yOffset4, startZ4);
    TopoDS_Shape paz4 = BRepPrimAPI_MakeBox(origin4, width4, depth4, length4_paz).Shape();
    BRepAlgoAPI_Cut cutPaz4(shaft, paz4);
    shaft = cutPaz4.Shape();

    // Паз на 10-м цилиндре: длина 22 мм, начало на Z=133 мм
    Standard_Real width10 = 3.0; // Ширина паза
    Standard_Real depth10 = 3.0; // Глубина паза (для Ø27 мм радиус 13.5 мм, нижняя часть паза на 10.5 мм)
    Standard_Real length10_paz = 22.0; // Длина паза вдоль оси Z
    Standard_Real startZ10 = 133.0; // Начало паза по Z (133 мм от левого края)
    Standard_Real yOffset10 = radius10 - depth10; // Смещение по Y (13.5 мм - 3 мм = 10.5 мм)
    gp_Pnt origin10(-width10 / 2.0, yOffset10, startZ10);
    TopoDS_Shape paz10 = BRepPrimAPI_MakeBox(origin10, width10, depth10, length10_paz).Shape();
    BRepAlgoAPI_Cut cutPaz10(shaft, paz10);
    shaft = cutPaz10.Shape();

    // Экспортируем в STEP
    STEPControl_Writer writer;
    writer.Transfer(shaft, STEPControl_AsIs);
    writer.Write("shaft.step");

    return 0;
}
