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

/**
 * @class ShaftSegmentProportion
 * @brief Структура для хранения пропорций сегмента вала
 */
struct ShaftSegmentProportion {
    std::string name;         // Название сегмента (например, "Цилиндр 1")
    std::string type;         // Тип сегмента ("cylinder" или "cone")
    double lengthRatio;       // Отношение длины к базовой длине
    double diameterRatio;     // Отношение диаметра к базовому диаметру
    double diameterEndRatio;  // Отношение конечного диаметра к базовому (для конуса)
    bool needsReduction;      // Требуется ли уменьшение диаметра на 0.3 мм

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

/**
 * @class SlotProportion
 * @brief Структура для хранения пропорций паза
 */
struct SlotProportion {
    double width;           // Ширина паза
    double depth;           // Глубина паза
    double length;          // Длина паза
    double positionRatio;   // Отношение позиции начала паза к общей длине вала
    int segmentIndex;       // Индекс сегмента, на котором расположен паз

    SlotProportion(double width, double depth, double length,
                   double positionRatio, int segmentIndex)
        : width(width), depth(depth), length(length),
        positionRatio(positionRatio), segmentIndex(segmentIndex) {}
};

/**
 * @class ShaftProportions
 * @brief Класс для управления пропорциями вала
 */
class ShaftProportions {
private:
    std::vector<ShaftSegmentProportion> proportions;
    double baseDiameter;           // Базовый диаметр для расчета пропорций
    double baseLength;             // Базовая длина для расчета пропорций
    std::map<int, double> customDiameters;  // Пользовательские диаметры для конкретных сегментов
    double chamferLengthRatio;     // Отношение длины фаски к базовому диаметру
    std::vector<SlotProportion> slotProportions; // Пропорции пазов
    double totalLengthRatio;                     // Суммарное отношение длин всех сегментов


public:
    /**
     * @brief Конструктор с инициализацией стандартных пропорций
     * @param baseDiameter Базовый диаметр, от которого рассчитываются остальные
     */
    ShaftProportions(double baseDiameter = 23.0)
        : baseDiameter(baseDiameter), baseLength(baseDiameter * 1.0),
        chamferLengthRatio(0.025 / 23.0) {
        initDefaultProportions();
        initDefaultSlotProportions();
    }

    /**
     * @brief Инициализация стандартных пропорций на основе исходной модели
     */
    void initDefaultProportions() {
        // Очищаем текущие пропорции
        proportions.clear();

        // Добавляем пропорции для всех сегментов вала
        // Цилиндр 1 (базовый с учетом фаски)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 1", "cylinder", 18.0 / baseLength, 23.0 / baseDiameter));

        // Цилиндр 2
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 2", "cylinder", 15.0 / baseLength, 25.0 / baseDiameter));

        // Цилиндр 3 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 3", "cylinder", 3.0 / baseLength, 23.0 / baseDiameter, 0.0, true));

        // Цилиндр 4
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 4", "cylinder", 27.0 / baseLength, 23.0 / baseDiameter));

        // Цилиндр 5 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 5", "cylinder", 3.0 / baseLength, 23.0 / baseDiameter, 0.0, true));

        // Цилиндр 6
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 6", "cylinder", 14.0 / baseLength, 35.0 / baseDiameter));

        // Конус
        proportions.push_back(ShaftSegmentProportion(
            "Конус", "cone", 5.0 / baseLength, 35.0 / baseDiameter, 40.0 / baseDiameter));

        // Цилиндр 7
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 7", "cylinder", 40.0 / baseLength, 40.0 / baseDiameter));

        // Цилиндр 8 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 8", "cylinder", 3.0 / baseLength, 23.0 / baseDiameter, 0.0, true));

        // Цилиндр 9
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 9", "cylinder", 59.0 / baseLength, 27.0 / baseDiameter));

        // Цилиндр 10
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 10", "cylinder", 3.0 / baseLength, 23.0 / baseDiameter));

        // Цилиндр 11
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 11", "cylinder", 21.0 / baseLength, 25.0 / baseDiameter));

        // Цилиндр 12 (с учетом фаски)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 12", "cylinder", 19.0 / baseLength, 23.0 / baseDiameter));
    }

    /**
     * @brief Установить пользовательский диаметр для сегмента
     * @param segmentIndex Индекс сегмента
     * @param diameter Новый диаметр
     */
    void setCustomDiameter(int segmentIndex, double diameter) {
        if (segmentIndex < 0 || segmentIndex >= proportions.size()) {
            throw std::out_of_range("Индекс сегмента вне допустимого диапазона");
        }
        customDiameters[segmentIndex] = diameter;

        // Если это первый заданный пользовательский диаметр, используем его как базовый
        if (customDiameters.size() == 1) {
            recalculateProportions();
        }
    }

    /**
     * @brief Пересчитать пропорции на основе пользовательских диаметров
     */
    void recalculateProportions() {
        if (customDiameters.empty()) {
            return; // Нет пользовательских диаметров для пересчета
        }

        // Берем первый заданный пользовательский диаметр как новый базовый
        auto firstCustom = customDiameters.begin();
        int firstIndex = firstCustom->first;
        double newDiameter = firstCustom->second;

        // Вычисляем коэффициент масштабирования
        // Если заданный диаметр для первого сегмента, используем его напрямую
        double scaleFactor;
        if (firstIndex == 0) {
            scaleFactor = newDiameter / baseDiameter;
        } else {
            scaleFactor = newDiameter / (baseDiameter * proportions[firstIndex].diameterRatio);
        }

        // Обновляем базовый диаметр
        baseDiameter *= scaleFactor;
        baseLength *= scaleFactor;

        std::cout << "Пересчет пропорций: новый базовый диаметр = " << baseDiameter
                  << ", коэффициент масштабирования = " << scaleFactor << std::endl;

        // Сбрасываем пользовательские диаметры, кроме первого заданного
        // Это предотвращает конфликты при масштабировании
        int referenceIndex = firstCustom->first;
        double referenceDiameter = firstCustom->second;
        customDiameters.clear();
        customDiameters[referenceIndex] = referenceDiameter;
    }

    /**
     * @brief Получить длину фаски для текущего базового диаметра
     * @return Длина фаски
     */
    double getChamferLength() const {
        return chamferLengthRatio * baseDiameter;
    }

    /**
     * @brief Получить количество сегментов
     * @return Количество сегментов
     */
    size_t getSegmentCount() const {
        return proportions.size();
    }

    /**
     * @brief Получить информацию о сегменте по индексу
     * @param index Индекс сегмента
     * @return Кортеж с типом, длиной, диаметром и флагом уменьшения
     */
    std::tuple<std::string, double, double, double, bool> getSegmentInfo(size_t index) const {
        if (index >= proportions.size()) {
            throw std::out_of_range("Индекс сегмента вне допустимого диапазона");
        }

        const auto& prop = proportions[index];
        double length = prop.lengthRatio * baseLength;

        // Проверяем, есть ли пользовательский диаметр для этого сегмента
        double diameter;
        if (customDiameters.find(index) != customDiameters.end()) {
            diameter = customDiameters.at(index);
        } else {
            diameter = prop.diameterRatio * baseDiameter;
        }

        double diameterEnd = 0.0;
        if (prop.type == "cone") {
            diameterEnd = prop.diameterEndRatio * baseDiameter;
        }

        return std::make_tuple(prop.type, length, diameter, diameterEnd, prop.needsReduction);
    }

    /**
     * @brief Получить название сегмента по индексу
     * @param index Индекс сегмента
     * @return Название сегмента
     */
    std::string getSegmentName(size_t index) const {
        if (index >= proportions.size()) {
            throw std::out_of_range("Индекс сегмента вне допустимого диапазона");
        }
        return proportions[index].name;
    }

    /**
 * @brief Получить диаметр сегмента по индексу
 * @param index Индекс сегмента
 * @return Диаметр сегмента
 */
    double getSegmentDiameter(size_t index) const {
        if (index >= proportions.size()) {
            throw std::out_of_range("Индекс сегмента вне допустимого диапазона");
        }

        const auto& prop = proportions[index];

        // Проверяем, есть ли пользовательский диаметр для этого сегмента
        if (customDiameters.find(index) != customDiameters.end()) {
            return customDiameters.at(index);
        } else {
            return prop.diameterRatio * baseDiameter;
        }
    }

    /**
     * @brief Инициализация пропорций пазов
     */
    void initDefaultSlotProportions() {
        // Очищаем текущие пропорции пазов
        slotProportions.clear();

        // Рассчитаем общую длину вала в пропорциях
        totalLengthRatio = 0.0;
        for (const auto& prop : proportions) {
            totalLengthRatio += prop.lengthRatio;
        }

        // Добавляем пропорции для пазов
        // Паз 1 на 4-м цилиндре (индекс 3)
        // 44.5 - позиция начала паза, 10.0 - длина паза
        // Нужно перевести абсолютные значения в относительные пропорции
        double length1Sum = 0.0;
        for (int i = 0; i < 3; i++) { // Суммируем длины до 4-го цилиндра
            length1Sum += proportions[i].lengthRatio;
        }
        double position1Ratio = (length1Sum + (44.5 - 36.0) / 27.0 * proportions[3].lengthRatio) / totalLengthRatio;
        double length1Ratio = 10.0 / (baseLength * totalLengthRatio);

        slotProportions.push_back(SlotProportion(
            8.0, 5.0, 10.0, position1Ratio, 3));

        // Паз 2 на 9-м цилиндре (индекс 9)
        // 133.0 - позиция начала паза, 22.0 - длина паза
        double length2Sum = 0.0;
        for (int i = 0; i < 9; i++) { // Суммируем длины до 10-го цилиндра
            length2Sum += proportions[i].lengthRatio;
        }
        double position2Ratio = (length2Sum + (133.0 - 125.0) / 59.0 * proportions[9].lengthRatio) / totalLengthRatio;
        double length2Ratio = 22.0 / (baseLength * totalLengthRatio);

        slotProportions.push_back(SlotProportion(
            8.0, 4.0, 22.0, position2Ratio, 9));
    }

    /**
     * @brief Получить количество пазов
     * @return Количество пазов
     */
    size_t getSlotCount() const {
        return slotProportions.size();
    }

    /**
     * @brief Получить информацию о пазе по индексу
     * @param index Индекс паза
     * @return Кортеж с шириной, глубиной, длиной, позицией и индексом сегмента
     */
    std::tuple<double, double, double, double, int> getSlotInfo(size_t index) const {
        if (index >= slotProportions.size()) {
            throw std::out_of_range("Индекс паза вне допустимого диапазона");
        }

        const auto& slot = slotProportions[index];

        // Рассчитываем абсолютную позицию паза
        double totalLength = baseLength * totalLengthRatio;
        double position = slot.positionRatio * totalLength;

        // Масштабируем размеры паза
        double scaledWidth = slot.width * (baseDiameter / 23.0); // Масштабируем относительно базового диаметра
        double scaledDepth = slot.depth * (baseDiameter / 23.0);
        double scaledLength = slot.length * (baseDiameter / 23.0);

        return std::make_tuple(scaledWidth, scaledDepth, scaledLength, position, slot.segmentIndex);
    }

    /**
     * @brief Получить общую длину вала
     * @return Общая длина вала
     */
    double getTotalLength() const {
        return baseLength * totalLengthRatio;
    }
};


#endif // SHAFT_PROPORTIONS_H
