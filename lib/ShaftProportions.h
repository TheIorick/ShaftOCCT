/**
 * @file ShaftProportions.h
 * @brief Класс для хранения пропорций вала и расчета размеров
 */

#ifndef SHAFT_PROPORTIONS_H
#define SHAFT_PROPORTIONS_H

#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <tuple>
#include <cmath>

struct ShaftSegmentProportion {
    std::string name;
    std::string type;
    double lengthRatio;
    double diameterRatio;
    double diameterEndRatio;
    bool needsReduction;

    ShaftSegmentProportion(
        const std::string& name,
        const std::string& type,
        double lengthRatio,
        double diameterRatio,
        double diameterEndRatio = 0.0,
        bool needsReduction = false)
        : name(name), type(type), lengthRatio(lengthRatio),
        diameterRatio(diameterRatio), diameterEndRatio(diameterEndRatio),
        needsReduction(needsReduction) {}
};

struct SlotProportion {
    double width;
    double depth;
    double length;
    double offsetFromSegmentStart;
    int segmentIndex;

    SlotProportion(double width, double depth, double length,
                   double offsetFromSegmentStart, int segmentIndex)
        : width(width), depth(depth), length(length),
        offsetFromSegmentStart(offsetFromSegmentStart), segmentIndex(segmentIndex) {}
};

class ShaftProportions {
private:
    std::vector<ShaftSegmentProportion> proportions;
    double baseDiameter;
    double totalLength;
    std::map<int, double> customDiameters;
    double chamferLengthRatio;
    std::vector<SlotProportion> slotProportions;
    double totalLengthRatio;

public:
    ShaftProportions(double totalLength = 230.0, double cylinder4Diameter = 23.0, double cylinder9Diameter = 27.0)
        : totalLength(totalLength), baseDiameter(23.0), chamferLengthRatio(0.025 / 23.0) {
        initDefaultProportions();
        customDiameters[3] = cylinder4Diameter;
        customDiameters[9] = cylinder9Diameter;
        recalculateProportions();
        initDefaultSlotProportions();
    }

    void initDefaultProportions() {
        proportions.clear();
        proportions.push_back(ShaftSegmentProportion("Cylinder 1", "cylinder", 18.0 / 230.0, 23.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 2", "cylinder", 15.0 / 230.0, 25.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 3", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));
        proportions.push_back(ShaftSegmentProportion("Cylinder 4", "cylinder", 27.0 / 230.0, 23.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 5", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));
        proportions.push_back(ShaftSegmentProportion("Cylinder 6", "cylinder", 14.0 / 230.0, 35.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Конус", "cone", 5.0 / 230.0, 35.0 / 23.0, 40.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 7", "cylinder", 40.0 / 230.0, 40.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 8", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));
        proportions.push_back(ShaftSegmentProportion("Cylinder 9", "cylinder", 59.0 / 230.0, 27.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 10", "cylinder", 3.0 / 230.0, 23.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 11", "cylinder", 21.0 / 230.0, 25.0 / 23.0));
        proportions.push_back(ShaftSegmentProportion("Cylinder 12", "cylinder", 19.0 / 230.0, 23.0 / 23.0));
        totalLengthRatio = 1.0;
    }

    void setCustomDiameter(int segmentIndex, double diameter) {
        if (segmentIndex < 0 || segmentIndex >= proportions.size()) {
            throw std::out_of_range("Segment index out of valid range");
        }
        if (diameter <= 20.0) throw std::invalid_argument("Diameter must be a positive number");
        if (diameter > 35.0) throw std::invalid_argument("Diameter cannot exceed 50 mm");
        customDiameters[segmentIndex] = diameter;
        if (segmentIndex == 3 || segmentIndex == 9) recalculateProportions();
    }

    void recalculateProportions() {
        if (customDiameters.empty()) return;
        if (customDiameters.find(3) != customDiameters.end()) {
            double cylinder4Diameter = customDiameters[3];
            if (cylinder4Diameter <= 0) {
                std::cerr << "Error: 4th cylinder diameter cannot be zero or negative" << std::endl;
                return;
            }
            double newBaseDiameter = cylinder4Diameter / proportions[3].diameterRatio;
            if (newBaseDiameter <= 0) {
                std::cerr << "Error: Calculated base diameter cannot be zero or negative" << std::endl;
                return;
            }
            double scaleFactor = newBaseDiameter / baseDiameter;
            baseDiameter = newBaseDiameter;
            std::cout << "Recalculating proportions based on the 4th cylinder diameter: "
                      << "new base diameter = " << baseDiameter
                      << ", scaling factor = " << scaleFactor << std::endl;
        } else if (customDiameters.find(9) != customDiameters.end()) {
            double cylinder9Diameter = customDiameters[9];
            if (cylinder9Diameter <= 0) {
                std::cerr << "Error: 9th cylinder diameter cannot be zero or negative" << std::endl;
                return;
            }
            double ratio = proportions[9].diameterRatio;
            if (ratio <= 0) {
                std::cerr << "Error: Diameter ratio for the 9th cylinder cannot be zero or negative" << std::endl;
                return;
            }
            double newBaseDiameter = cylinder9Diameter / ratio;
            if (newBaseDiameter <= 0) {
                std::cerr << "Error: Calculated base diameter cannot be zero or negative" << std::endl;
                return;
            }
            double scaleFactor = newBaseDiameter / baseDiameter;
            baseDiameter = newBaseDiameter;
            std::cout << "Recalculating proportions based on the 9th cylinder diameter: "
                      << "new base diameter = " << baseDiameter
                      << ", scaling factor = " << scaleFactor << std::endl;
        }
    }

    double getChamferLength() const { return chamferLengthRatio * baseDiameter; }

    size_t getSegmentCount() const { return proportions.size(); }

    std::tuple<std::string, double, double, double, bool> getSegmentInfo(size_t index) const {
        if (index >= proportions.size()) throw std::out_of_range("Segment index out of valid range");
        const auto& prop = proportions[index];
        double length = prop.lengthRatio * totalLength;
        double diameter = customDiameters.find(index) != customDiameters.end() ? customDiameters.at(index) : prop.diameterRatio * baseDiameter;
        double diameterEnd = prop.type == "cone" ? prop.diameterEndRatio * baseDiameter : 0.0;
        return std::make_tuple(prop.type, length, diameter, diameterEnd, prop.needsReduction);
    }

    std::string getSegmentName(size_t index) const {
        if (index >= proportions.size()) throw std::out_of_range("Segment index out of valid range");
        return proportions[index].name;
    }

    double getSegmentDiameter(size_t index) const {
        if (index >= proportions.size()) throw std::out_of_range("Segment index out of valid range");
        const auto& prop = proportions[index];
        return customDiameters.find(index) != customDiameters.end() ? customDiameters.at(index) : prop.diameterRatio * baseDiameter;
    }

    void initDefaultSlotProportions() {
        slotProportions.clear();
        slotProportions.push_back(SlotProportion(8.0, 5.0, 10.0, 8.5, 3));
        slotProportions.push_back(SlotProportion(8.0, 4.0, 22.0, 8.0, 9));
    }

    size_t getSlotCount() const { return slotProportions.size(); }

    std::tuple<double, double, double, double, int> getSlotInfo(size_t index) const {
        if (index >= slotProportions.size()) throw std::out_of_range("Slot index out of valid range");
        const auto& slot = slotProportions[index];
        double offset = slot.offsetFromSegmentStart;
        double scaledWidth = slot.width * (baseDiameter / 23.0);
        double scaledDepth = slot.depth * (baseDiameter / 23.0);
        double scaledLength = slot.length * (baseDiameter / 23.0);
        return std::make_tuple(scaledWidth, scaledDepth, scaledLength, offset, slot.segmentIndex);
    }

    double getTotalLength() const { return totalLength; }

    void setTotalLength(double length) {
        if (length <= 20) throw std::invalid_argument("Total shaft length must be positive");
        if (length > 300.0) throw std::invalid_argument("Total shaft length cannot exceed 300 mm");
        totalLength = length;
        initDefaultSlotProportions();
    }

    double getBaseDiameter() const { return baseDiameter; }
};

#endif // SHAFT_PROPORTIONS_H
