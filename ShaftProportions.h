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
    double lengthRatio;       // Отношение длины к общей длине вала
    double diameterRatio;     // Отношение диаметра к диаметру базового цилиндра
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
    double totalLength;            // Общая длина вала
    std::map<int, double> customDiameters;  // Пользовательские диаметры для конкретных сегментов
    double chamferLengthRatio;     // Отношение длины фаски к базовому диаметру
    std::vector<SlotProportion> slotProportions; // Пропорции пазов
    double totalLengthRatio;       // Суммарное отношение длин всех сегментов (всегда 1.0)

public:
    /**
     * @brief Конструктор с инициализацией пропорций на основе общей длины и диаметров ключевых цилиндров
     * @param totalLength Общая длина вала
     * @param cylinder4Diameter Диаметр 4-го цилиндра (с пазом)
     * @param cylinder9Diameter Диаметр 9-го цилиндра (с пазом)
     */
    ShaftProportions(double totalLength = 230.0, double cylinder4Diameter = 23.0, double cylinder9Diameter = 27.0)
        : totalLength(totalLength), baseDiameter(23.0), chamferLengthRatio(0.025 / 23.0) {
        // Инициализируем пропорции
        initDefaultProportions();

        // Установка пользовательских диаметров для 4-го и 9-го цилиндров
        customDiameters[3] = cylinder4Diameter; // 4-й цилиндр (индекс 3)
        customDiameters[8] = cylinder9Diameter; // 9-й цилиндр (индекс 8)

        // Рассчитываем базовый диаметр на основе диаметра 4-го цилиндра
        recalculateProportions();

        // Инициализируем пазы
        initDefaultSlotProportions();
    }

    /**
     * @brief Инициализация стандартных пропорций на основе исходной модели
     */
    void initDefaultProportions() {
        // Очищаем текущие пропорции
        proportions.clear();

        // Добавляем пропорции для всех сегментов вала
        // Теперь все длины в пропорциях к общей длине вала

        // Цилиндр 1
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 1", "cylinder", 18.0 / 230.0, 23.0 / 23.0));

        // Цилиндр 2
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 2", "cylinder", 15.0 / 230.0, 25.0 / 23.0));

        // Цилиндр 3 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 3", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));

        // Цилиндр 4 (с пазом)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 4", "cylinder", 27.0 / 230.0, 23.0 / 23.0));

        // Цилиндр 5 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 5", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));

        // Цилиндр 6
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 6", "cylinder", 14.0 / 230.0, 35.0 / 23.0));

        // Конус
        proportions.push_back(ShaftSegmentProportion(
            "Конус", "cone", 5.0 / 230.0, 35.0 / 23.0, 40.0 / 23.0));

        // Цилиндр 7
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 7", "cylinder", 40.0 / 230.0, 40.0 / 23.0));

        // Цилиндр 8 (с уменьшенным диаметром)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 8", "cylinder", 3.0 / 230.0, 23.0 / 23.0, 0.0, true));

        // Цилиндр 9 (с пазом)
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 9", "cylinder", 59.0 / 230.0, 27.0 / 23.0));

        // Цилиндр 10
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 10", "cylinder", 3.0 / 230.0, 23.0 / 23.0));

        // Цилиндр 11
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 11", "cylinder", 21.0 / 230.0, 25.0 / 23.0));

        // Цилиндр 12
        proportions.push_back(ShaftSegmentProportion(
            "Цилиндр 12", "cylinder", 19.0 / 230.0, 23.0 / 23.0));

        // Общая пропорция длины должна быть равна 1.0
        totalLengthRatio = 1.0;
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
        if (diameter <= 0) {
            throw std::invalid_argument("Диаметр должен быть положительным числом");
        }

        customDiameters[segmentIndex] = diameter;

        // Пересчитываем пропорции при изменении диаметра
        if (segmentIndex == 3 || segmentIndex == 8) {  // 4-й или 9-й цилиндр
            recalculateProportions();
        }
    }

    /**
     * @brief Пересчитать пропорции на основе пользовательских диаметров
     * В новой модели пересчет производится на основе диаметра 4-го цилиндра (индекс 3)
     */
    void recalculateProportions() {
        // Если нет пользовательских диаметров, нечего пересчитывать
        if (customDiameters.empty()) {
            return;
        }

        // Приоритет отдаем 4-му цилиндру (индекс 3), если он задан
        if (customDiameters.find(3) != customDiameters.end()) {
            double cylinder4Diameter = customDiameters[3];

            // Проверяем, что диаметр не ноль
            if (cylinder4Diameter <= 0) {
                std::cerr << "Ошибка: диаметр 4-го цилиндра не может быть нулевым или отрицательным" << std::endl;
                return;
            }

            // Рассчитываем новый базовый диаметр исходя из диаметра 4-го цилиндра
            // Для 4-го цилиндра (индекс 3) отношение diameterRatio = 23.0 / 23.0 = 1.0
            double newBaseDiameter = cylinder4Diameter / proportions[3].diameterRatio;

            // Проверяем, что получившийся базовый диаметр не ноль
            if (newBaseDiameter <= 0) {
                std::cerr << "Ошибка: вычисленный базовый диаметр не может быть нулевым или отрицательным" << std::endl;
                return;
            }

            // Обновляем базовый диаметр
            double scaleFactor = newBaseDiameter / baseDiameter;
            baseDiameter = newBaseDiameter;

            std::cout << "Пересчет пропорций на основе диаметра 4-го цилиндра: "
                      << "новый базовый диаметр = " << baseDiameter
                      << ", коэффициент масштабирования = " << scaleFactor << std::endl;
        }
        // Если 4-й цилиндр не задан, но задан 9-й цилиндр (индекс 8)
        else if (customDiameters.find(8) != customDiameters.end()) {
            double cylinder9Diameter = customDiameters[8];

            // Проверяем, что диаметр не ноль
            if (cylinder9Diameter <= 0) {
                std::cerr << "Ошибка: диаметр 9-го цилиндра не может быть нулевым или отрицательным" << std::endl;
                return;
            }

            // Для 9-го цилиндра (индекс 8) отношение diameterRatio = 27.0 / 23.0
            double ratio = proportions[8].diameterRatio;
            if (ratio <= 0) {
                std::cerr << "Ошибка: соотношение диаметра для 9-го цилиндра не может быть нулевым или отрицательным" << std::endl;
                return;
            }

            // Рассчитываем новый базовый диаметр
            double newBaseDiameter = cylinder9Diameter / ratio;

            // Проверяем, что получившийся базовый диаметр не ноль
            if (newBaseDiameter <= 0) {
                std::cerr << "Ошибка: вычисленный базовый диаметр не может быть нулевым или отрицательным" << std::endl;
                return;
            }

            // Обновляем базовый диаметр
            double scaleFactor = newBaseDiameter / baseDiameter;
            baseDiameter = newBaseDiameter;

            std::cout << "Пересчет пропорций на основе диаметра 9-го цилиндра: "
                      << "новый базовый диаметр = " << baseDiameter
                      << ", коэффициент масштабирования = " << scaleFactor << std::endl;
        }
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
        double length = prop.lengthRatio * totalLength;

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
        double position1Ratio = (length1Sum + (44.5 - 36.0) / totalLength) / totalLengthRatio;
        double length1Ratio = 10.0 / (baseDiameter * totalLengthRatio);

        slotProportions.push_back(SlotProportion(
            8.0, 5.0, 10.0, position1Ratio, 3));

        // Паз 2 на 9-м цилиндре (индекс 8)
        // 133.0 - позиция начала паза, 22.0 - длина паза
        double length2Sum = 0.0;
        for (int i = 0; i < 8; i++) { // Суммируем длины до 9-го цилиндра
            length2Sum += proportions[i].lengthRatio;
        }
        double position2Ratio = (length2Sum + (133.0 - 125.0) / totalLength) / totalLengthRatio;
        double length2Ratio = 22.0 / (baseDiameter * totalLengthRatio);

        slotProportions.push_back(SlotProportion(
            8.0, 4.0, 22.0, position2Ratio, 8));
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
        double position = slot.positionRatio * totalLength;

        // Масштабируем размеры паза относительно базового диаметра
        double scaledWidth = slot.width * (baseDiameter / 23.0);
        double scaledDepth = slot.depth * (baseDiameter / 23.0);
        double scaledLength = slot.length * (baseDiameter / 23.0);

        return std::make_tuple(scaledWidth, scaledDepth, scaledLength, position, slot.segmentIndex);
    }

    /**
     * @brief Получить общую длину вала
     * @return Общая длина вала
     */
    double getTotalLength() const {
        return totalLength;
    }

    /**
     * @brief Установить общую длину вала
     * @param length Новая общая длина
     */
    void setTotalLength(double length) {
        if (length <= 0) {
            throw std::invalid_argument("Общая длина вала должна быть положительной");
        }
        totalLength = length;

        // После изменения общей длины обновляем позиции пазов
        initDefaultSlotProportions();
    }

    /**
     * @brief Получить базовый диаметр
     * @return Базовый диаметр
     */
    double getBaseDiameter() const {
        return baseDiameter;
    }
};

#endif // SHAFT_PROPORTIONS_H
