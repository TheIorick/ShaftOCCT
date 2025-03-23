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
     * @param chamferLength Длина фаски
     * @param chamferAngle Угол фаски в градусах
     */
    ShaftApplication(Standard_Real chamferLength = 0.025, Standard_Real chamferAngle = 45.0)
        : builder(chamferLength, chamferAngle), proportions(23.0)  {}

    /**
     * @brief Запустить построение вала
     * @param exportFilename Имя файла для экспорта
     * @return 0 при успешном выполнении, иначе код ошибки
     */
    int run(const std::string& exportFilename = "shaft.step") {
        std::cout << "Начинаем построение вала на основе пропорций..." << std::endl;

        try {
            // Добавляем пользовательские диаметры (если есть)
            // Например, если известен только диаметр первого цилиндра:
            // proportions.setCustomDiameter(0, 23.0); // Первый цилиндр имеет диаметр 23.0

            // Если нужно задать несколько диаметров:
            // proportions.setCustomDiameter(7, 40.0); // Цилиндр 7 имеет диаметр 40.0

            // Строим вал на основе пропорций
            builder.buildFromProportions(proportions);

            // Строим модель вала
            builder.build();

            // Экспортируем модель в STEP-файл
            builder.exportToSTEP(exportFilename);

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
};

/**
 * @brief Главная функция программы
 * @return 0 при успешном выполнении, иначе код ошибки
 */
int main() {
    // Общие параметры
    Standard_Real chamferLength = 0.025;  // Длина фаски
    Standard_Real chamferAngle = 45.0;    // Угол фаски в градусах

    // Создаем приложение
    ShaftApplication app(chamferLength, chamferAngle);

    // Вариант 1: Используем стандартные пропорции с базовым диаметром 23.0 мм
    // return app.run("shaft_standard.step");

    // Вариант 2: Задаем пользовательский диаметр для одного сегмента
    app.setSegmentDiameter(0, 35.0); // Увеличиваем диаметр
    return app.run("shaft_custom_diameter.step");

    // Вариант 3: Задаем пользовательские диаметры для нескольких сегментов
    // app.setSegmentDiameter(1, 30.0); // Первый цилиндр
    // app.setSegmentDiameter(7, 50.0); // Цилиндр 7
    // return app.run("shaft_multi_custom.step");
}
