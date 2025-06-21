#include "ShaftAppCore.h"
#include <iostream>
#include <Standard_DefineAlloc.hxx>

/**
 * @brief Конструктор
 */
ShaftAppCore::ShaftAppCore(
    double totalLength,
    double cylinder4Diameter,
    double cylinder9Diameter,
    Standard_Real chamferLength,
    Standard_Real chamferAngle)
    : builder(chamferLength, chamferAngle),
    proportions(totalLength, cylinder4Diameter, cylinder9Diameter),
    m_hasConfigurationErrors(false) {}

/**
 * @brief Запустить построение вала
 */
int ShaftAppCore::run(const std::string& exportFilename) {
    resetConfigurationErrors();
    if (m_hasConfigurationErrors) {
        std::cerr << "Cannot build shaft due to configuration errors. Please fix them first." << std::endl;
        return 1;
    }
    std::cout << "Starting shaft construction with total length "
              << proportions.getTotalLength() << " mm..." << std::endl;

    try {
        builder.buildFromProportions(proportions);
        builder.build();
        builder.exportToSTEP(exportFilename);
        std::cout << "Shaft construction completed successfully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error during shaft construction: " << e.what() << std::endl;
        return 1;
    }
}

/**
 * @brief Задать диаметр для указанного сегмента
 */
void ShaftAppCore::setSegmentDiameter(int segmentIndex, double diameter) {
    m_hasConfigurationErrors = false;
    try {
        proportions.setCustomDiameter(segmentIndex, diameter);
        std::cout << "Diameter set to " << diameter << " mm for segment "
                  << proportions.getSegmentName(segmentIndex) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error setting diameter: " << e.what() << std::endl;
        m_hasConfigurationErrors = true;
    }
}

/**
 * @brief Задать общую длину вала
 */
void ShaftAppCore::setTotalLength(double length) {
    m_hasConfigurationErrors = false;
    try {
        proportions.setTotalLength(length);
        std::cout << "Total shaft length set to: " << length << " mm" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error setting shaft length: " << e.what() << std::endl;
        m_hasConfigurationErrors = true;
    }
}

/**
 * @brief Сбрасывает флаг ошибок конфигурации
 */
void ShaftAppCore::resetConfigurationErrors() {
    m_hasConfigurationErrors = false;
}
