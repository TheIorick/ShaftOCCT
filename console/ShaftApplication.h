#ifndef SHAFT_APPLICATION_H
#define SHAFT_APPLICATION_H

#include "../lib/ShaftBuilder.h"
#include "../lib/ShaftProportions.h"
#include <string>

/**
 * @class ShaftApplication
 * @brief Класс приложения для построения вала
 */
class ShaftApplication {
private:
    ShaftBuilder builder;
    ShaftProportions proportions;

public:
    /**
     * @brief Конструктор приложения
     * @param totalLength Общая длина вала
     * @param cylinder4Diameter Диаметр 4-го цилиндра (с пазом)
     * @param cylinder9Diameter Диаметр 9-го цилиндра (с пазом)
     * @param chamferLength Длина фаски
     * @param chamferAngle Угол фаски в градусах
     */
    ShaftApplication(
        double totalLength = 230.0,
        double cylinder4Diameter = 23.0,
        double cylinder9Diameter = 27.0,
        Standard_Real chamferLength = 0.025,
        Standard_Real chamferAngle = 45.0);

    /**
     * @brief Запустить построение вала
     * @param exportFilename Имя файла для экспорта
     * @return 0 при успешном выполнении, иначе код ошибки
     */
    int run(const std::string& exportFilename = "shaft.step");

    /**
     * @brief Задать диаметр для указанного сегмента
     * @param segmentIndex Индекс сегмента (начиная с 0)
     * @param diameter Диаметр сегмента
     */
    void setSegmentDiameter(int segmentIndex, double diameter);

    /**
     * @brief Задать общую длину вала
     * @param length Общая длина вала
     */
    void setTotalLength(double length);
};

#endif // SHAFT_APPLICATION_H
