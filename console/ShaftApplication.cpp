#include "ShaftApplication.h"
#include <iostream>

/**
 * @brief Конструктор
 */
ShaftApplication::ShaftApplication(
    double totalLength,
    double cylinder4Diameter,
    double cylinder9Diameter,
    Standard_Real chamferLength,
    Standard_Real chamferAngle)
    : core(totalLength, cylinder4Diameter, cylinder9Diameter, chamferLength, chamferAngle) {}

/**
 * @brief Запустить построение вала
 */
int ShaftApplication::run(const std::string& exportFilename) {
    return core.run(exportFilename);
}

/**
 * @brief Задать диаметр для указанного сегмента
 */
void ShaftApplication::setSegmentDiameter(int segmentIndex, double diameter) {
    core.setSegmentDiameter(segmentIndex, diameter);
}

/**
 * @brief Задать общую длину вала
 */
void ShaftApplication::setTotalLength(double length) {
    core.setTotalLength(length);
}

/**
 * @brief Главная функция
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
        if (argc > 1) totalLength = std::stod(argv[1]);
        if (argc > 2) cylinder4Diameter = std::stod(argv[2]);
        if (argc > 3) cylinder9Diameter = std::stod(argv[3]);

        if (totalLength <= 0 || cylinder4Diameter <= 0 || cylinder9Diameter <= 0) {
            std::cerr << "Error: All parameters must be positive numbers" << std::endl;
            return 1;
        }
    }

    std::cout << "Shaft parameters:" << std::endl;
    std::cout << "Total length: " << totalLength << " mm" << std::endl;
    std::cout << "Diameter of the 4th cylinder: " << cylinder4Diameter << " mm" << std::endl;
    std::cout << "Diameter of the 9th cylinder: " << cylinder9Diameter << " mm" << std::endl;

    ShaftApplication app(totalLength, cylinder4Diameter, cylinder9Diameter, chamferLength, chamferAngle);
    return app.run("shaft_custom_dimensions.step");
}
