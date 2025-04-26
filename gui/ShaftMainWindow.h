/**
 * @file ShaftMainWindow.h
 * @brief Главное окно графического интерфейса для приложения ShaftOCCT
 */

#ifndef SHAFT_MAIN_WINDOW_H
#define SHAFT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include "ShaftBuilder.h"
#include "ShaftProportions.h"

/**
 * @class ShaftWidget
 * @brief Виджет для отображения 3D модели вала
 */
class ShaftWidget : public QWidget {
    Q_OBJECT
public:
    explicit ShaftWidget(QWidget *parent = nullptr);
    void setShape(const TopoDS_Shape& shape);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    TopoDS_Shape m_shape; // 3D модель вала
    bool m_hasShape;      // Флаг наличия модели
};

/**
 * @class ShaftMainWindow
 * @brief Главное окно приложения
 */
class ShaftMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ShaftMainWindow(QWidget *parent = nullptr);
    ~ShaftMainWindow();

private slots:
    void buildShaft();
    void exportShaft();

private:
    void setupUI();

    ShaftWidget *m_shaftWidget;      // Виджет для отображения вала
    QLineEdit *m_lengthEdit;         // Поле ввода длины вала
    QLineEdit *m_diameterEdit;       // Поле ввода максимального диаметра
    QPushButton *m_buildButton;      // Кнопка построения модели
    QPushButton *m_exportButton;     // Кнопка экспорта модели

    ShaftBuilder m_builder;          // Строитель вала
    ShaftProportions m_proportions;  // Пропорции вала
};

#endif // SHAFT_MAIN_WINDOW_H
