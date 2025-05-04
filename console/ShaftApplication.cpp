/**
 * @file ShaftApplication.cpp
 * @brief Приложение для создания и экспорта 3D модели вала
 */

#include "ShaftApplication.h"
#include <iostream>

/**
 * @brief Конструктор приложения
 */
ShaftApplication::ShaftApplication(
    double totalLength,
    double cylinder4Diameter,
    double cylinder9Diameter,
    Standard_Real chamferLength,
    Standard_Real chamferAngle)
    : builder(chamferLength, chamferAngle),
    proportions(totalLength, cylinder4Diameter, cylinder9Diameter) {}

/**
 * @brief Запустить построение вала
 */
int ShaftApplication::run(const std::string& exportFilename) {
    std::cout << "Starting shaft construction with total length "
              << proportions.getTotalLength() << " mm..." << std::endl;

    try {
        // Строим вал на основе пропорций
        builder.buildFromProportions(proportions);

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

/**
 * @brief Задать диаметр для указанного сегмента
 */
void ShaftApplication::setSegmentDiameter(int segmentIndex, double diameter) {
    try {
        proportions.setCustomDiameter(segmentIndex, diameter);
        std::cout << "Diameter set to " << diameter << " mm for segment "
                  << proportions.getSegmentName(segmentIndex) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error setting diameter: " << e.what() << std::endl;
    }
}

/**
 * @brief Задать общую длину вала
 */
void ShaftApplication::setTotalLength(double length) {
    try {
        proportions.setTotalLength(length);
        std::cout << "Total shaft length set to: " << length << " mm" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error setting shaft length: " << e.what() << std::endl;
    }
}

/**
 * @brief Главная функция программы
 */
int main(int argc, char *argv[]) {
    double totalLength = 230.0;
    double cylinder4Diameter = 23.0;
    double cylinder9Diameter = 27.0;
    Standard_Real chamferLength = 0.025;
    Standard_Real chamferAngle = 45.0;

    if (argc == 1) {
        std::cout << "Enter the total length of the shaft (mm): ";
        std::cin >> totalLength;
        if (totalLength <= 0) {
            std::cerr << "Error: The shaft length must be a positive number" << std::endl;
            return 1;
        }

        std::cout << "Enter the diameter of the 4th cylinder (mm): ";
        std::cin >> cylinder4Diameter;
        if (cylinder4Diameter <= 0) {
            std::cerr << "Error: The diameter of the 4th cylinder must be a positive number" << std::endl;
            return 1;
        }

        std::cout << "Enter the diameter of the 9th cylinder (mm): ";
        std::cin >> cylinder9Diameter;
        if (cylinder9Diameter <= 0) {
            std::cerr << "Error: The diameter of the 9th cylinder must be a positive number" << std::endl;
            return 1;
        }
    } else {
        if (argc > 1) {
            totalLength = std::stod(argv[1]);
            if (totalLength <= 0) {
                std::cerr << "Error: The shaft length must be a positive number" << std::endl;
                return 1;
            }
        }
        if (argc > 2) {
            cylinder4Diameter = std::stod(argv[2]);
            if (cylinder4Diameter <= 0) {
                std::cerr << "Error: The diameter of the 4th cylinder must be a positive number" << std::endl;
                return 1;
            }
        }
        if (argc > 3) {
            cylinder9Diameter = std::stod(argv[3]);
            if (cylinder9Diameter <= 0) {
                std::cerr << "Error: The diameter of the 9th cylinder must be a positive number" << std::endl;
                return 1;
            }
        }
    }

    std::cout << "Shaft parameters:" << std::endl;
    std::cout << "Total length: " << totalLength << " mm" << std::endl;
    std::cout << "Diameter of the 4th cylinder: " << cylinder4Diameter << " mm" << std::endl;
    std::cout << "Diameter of the 9th cylinder: " << cylinder9Diameter << " mm" << std::endl;

    ShaftApplication app(totalLength, cylinder4Diameter, cylinder9Diameter, chamferLength, chamferAngle);
    return app.run("shaft_custom_dimensions.step");
}
