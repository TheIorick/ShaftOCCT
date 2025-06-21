#ifndef SHAFT_APP_CORE_H
#define SHAFT_APP_CORE_H

#include "ShaftBuilder.h"
#include "ShaftProportions.h"
#include <string>
#include <Standard_TypeDef.hxx>

/**
 * @class ShaftAppCore
 * @brief Основной класс для построения вала, переиспользуемый в консольных и GUI приложениях
 */
class ShaftAppCore {
private:
    ShaftBuilder builder;
    ShaftProportions proportions;
    bool m_hasConfigurationErrors;

public:
    /**
     * @brief Конструктор
     * @param totalLength Общая длина вала
     * @param cylinder4Diameter Диаметр 4-го цилиндра
     * @param cylinder9Diameter Диаметр 9-го цилиндра
     * @param chamferLength Длина фаски
     * @param chamferAngle Угол фаски в градусах
     */
    ShaftAppCore(
        double totalLength = 230.0,
        double cylinder4Diameter = 23.0,
        double cylinder9Diameter = 27.0,
        Standard_Real chamferLength = 0.025,
        Standard_Real chamferAngle = 45.0);

    /**
     * @brief Запустить построение вала
     * @param exportFilename Имя файла для экспорта
     * @return 0 при успехе, иначе код ошибки
     */
    int run(const std::string& exportFilename);

    /**
     * @brief Задать диаметр для указанного сегмента
     * @param segmentIndex Индекс сегмента
     * @param diameter Диаметр сегмента
     */
    void setSegmentDiameter(int segmentIndex, double diameter);

    /**
     * @brief Задать общую длину вала
     * @param length Общая длина вала
     */
    void setTotalLength(double length);

    /**
     * @brief Сбрасывает флаг ошибок конфигурации
     */
    void resetConfigurationErrors();
};

#endif // SHAFT_APP_CORE_H
