/**
 * @file ShaftApplication.cpp
 * @brief Приложение для создания и экспорта 3D модели вала
 */

#include "ShaftBuilder.h" // Подключаем класс ShaftBuilder
#include <iostream>

/**
 * @class ShaftApplication
 * @brief Класс приложения для построения вала
 */
class ShaftApplication {
private:
    ShaftBuilder builder; // Строитель вала

public:
    /**
     * @brief Конструктор приложения
     * @param chamferLength Длина фаски
     * @param chamferAngle Угол фаски в градусах
     */
    ShaftApplication(Standard_Real chamferLength = 0.025, Standard_Real chamferAngle = 45.0)
        : builder(chamferLength, chamferAngle) {}

    /**
     * @brief Запустить построение вала
     * @param exportFilename Имя файла для экспорта
     * @return 0 при успешном выполнении, иначе код ошибки
     */
    int run(const std::string& exportFilename = "shaft.step") {
        std::cout << "Starting shaft construction..." << std::endl;

        try {
            // Получаем длину фаски для учета в первом и последнем сегментах
            Standard_Real chamferLength = 0.025;

            // Создаем модель вала, добавляя последовательно все сегменты

            // Цилиндр 1 (с учетом фаски)
            builder.addCylinder(18.0 + chamferLength, 23.0);

            // Цилиндр 2
            builder.addCylinder(15.0, 25.0);

            // Цилиндр 3
            builder.addCylinder(3.0, 22.7);

            // Цилиндр 4
            builder.addCylinder(27.0, 23.0);

            // Цилиндр 5
            builder.addCylinder(3.0, 22.7);

            // Цилиндр 6
            builder.addCylinder(14.0, 35.0);

            // Конус
            builder.addCone(5.0, 35.0, 40.0);

            // Цилиндр 7
            builder.addCylinder(40.0, 40.0);

            // Цилиндр 8
            builder.addCylinder(3.0, 22.7);

            // Цилиндр 9
            builder.addCylinder(59.0, 27.0);

            // Цилиндр 10
            builder.addCylinder(3.0, 23.0);

            // Цилиндр 11
            builder.addCylinder(21.0, 25.0);

            // Цилиндр 12 (с учетом фаски)
            builder.addCylinder(19.0 + chamferLength, 23.0);

            // Добавляем пазы на вал
            // 1-й паз на 4-м цилиндре
            builder.addSlot(8.0, 5.0, 10.0, 44.5, 23.0/2.0);

            // 2-й паз на 10-м цилиндре
            builder.addSlot(8.0, 4.0, 22.0, 133.0, 27.0/2.0);

            // Строим модель вала
            builder.build();

            // Экспортируем модель в STEP-файл
            builder.exportToSTEP(exportFilename);

            std::cout << "Shaft construction completed successfully." << std::endl;
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Error during shaft construction: " << e.what() << std::endl;
            return 1;
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

    // Создаем и запускаем приложение
    ShaftApplication app(chamferLength, chamferAngle);
    return app.run("shaft.step");
}
