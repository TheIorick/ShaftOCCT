#ifndef SHAFT_APPLICATION_H
#define SHAFT_APPLICATION_H

#include "../lib/ShaftAppCore.h"
#include <string>

/**
 * @class ShaftApplication
 * @brief Класс консольного приложения для построения вала
 */
class ShaftApplication {
private:
    ShaftAppCore core;

public:
    ShaftApplication(
        double totalLength = 230.0,
        double cylinder4Diameter = 23.0,
        double cylinder9Diameter = 27.0,
        Standard_Real chamferLength = 0.025,
        Standard_Real chamferAngle = 45.0);

    int run(const std::string& exportFilename = "shaft.step");
    void setSegmentDiameter(int segmentIndex, double diameter);
    void setTotalLength(double length);
};

#endif // SHAFT_APPLICATION_H
