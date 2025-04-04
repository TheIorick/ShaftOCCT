/**
 * @file ShaftApplication.cpp
 * @brief Приложение для создания и экспорта 3D модели вала
 */

#include "ShaftBuilder.h" // Подключаем класс ShaftBuilder
#include "ShaftProportions.h"
#include <iostream>

/**
 * @class ShaftApplication
 * @brief Класс приложения для построения вала
 */
class ShaftApplication {
private:
    ShaftBuilder builder; // Строитель вала
    ShaftProportions proportions; // Пропорции вала

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
        Standard_Real chamferAngle = 45.0)
        : builder(chamferLength, chamferAngle),
        proportions(totalLength, cylinder4Diameter, cylinder9Diameter) {}

    /**
     * @brief Запустить построение вала
     * @param exportFilename Имя файла для экспорта
     * @return 0 при успешном выполнении, иначе код ошибки
     */
    int run(const std::string& exportFilename = "shaft.step") {
        std::cout << "Начинаем построение вала с общей длиной "
                  << proportions.getTotalLength() << " мм..." << std::endl;

        try {
            // Строим вал на основе пропорций
            builder.buildFromProportions(proportions);

            // Строим модель вала
            builder.build();

            // Экспортируем модель в STEP-файл
            // builder.exportToSTEP(exportFilename);

            std::cout << "Построение вала успешно завершено." << std::endl;
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при построении вала: " << e.what() << std::endl;
            return 1;
        }
    }


    /**
 * @brief Задать диаметр для указанного сегмента
 * @param segmentIndex Индекс сегмента (начиная с 0)
 * @param diameter Диаметр сегмента
 */
    void setSegmentDiameter(int segmentIndex, double diameter) {
        try {
            proportions.setCustomDiameter(segmentIndex, diameter);
            std::cout << "Установлен диаметр " << diameter << " мм для сегмента "
                      << proportions.getSegmentName(segmentIndex) << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при установке диаметра: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Задать общую длину вала
     * @param length Общая длина вала
     */
    void setTotalLength(double length) {
        try {
            proportions.setTotalLength(length);
            std::cout << "Установлена общая длина вала: " << length << " мм" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при установке длины вала: " << e.what() << std::endl;
        }
    }
};

/**
 * @brief Главная функция программы
 * @return 0 при успешном выполнении, иначе код ошибки
 */
int main() {

    // Общие параметры
    double totalLength = 230.0;           // Общая длина вала
    double cylinder4Diameter = 23.0;      // Диаметр 4-го цилиндра
    double cylinder9Diameter = 27.0;      // Диаметр 9-го цилиндра
    Standard_Real chamferLength = 0.025;  // Длина фаски
    Standard_Real chamferAngle = 45.0;    // Угол фаски в градусах

    // Создаем приложение
    ShaftApplication app(chamferLength, chamferAngle);

    // Вариант 1: Используем стандартные параметры
    // return app.run("shaft_standard.step");

    // Вариант 2: Задаем пользовательский диаметр для одного сегмента
    // app.setSegmentDiameter(0, 35.0); // Увеличиваем диаметр первого цилиндра
    // return app.run("shaft_custom_diameter.step");

    // Вариант 3: Задаем пользовательскую длину и диаметры
    app.setTotalLength(250.0); // Увеличиваем общую длину
    app.setSegmentDiameter(3, 25.0); // Увеличиваем диаметр 4-го цилиндра
    app.setSegmentDiameter(8, 30.0); // Увеличиваем диаметр 9-го цилиндра
    return app.run("shaft_custom_dimensions.step");
}
