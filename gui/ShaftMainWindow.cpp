/**
 * @file ShaftMainWindow.cpp
 * @brief Реализация главного окна графического интерфейса
 */

#include "ShaftMainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>

// Реализация ShaftWidget

ShaftWidget::ShaftWidget(QWidget *parent)
    : QWidget(parent), m_hasShape(false) {
    // Установка минимального размера для виджета отображения вала
    setMinimumSize(400, 300);
    // Установка предпочтительного размера
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ShaftWidget::setShape(const TopoDS_Shape& shape) {
    m_shape = shape;
    m_hasShape = true;
    update(); // Перерисовать виджет
}

void ShaftWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Заливка фона светло-голубым цветом
    painter.fillRect(rect(), QColor(200, 230, 255));

    // Рисуем оси координат
    int centerX = width() / 2;
    int centerY = height() / 2;
    int axisLength = 50;

    painter.setPen(QPen(Qt::blue, 2));

    // Ось X (красная)
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(centerX, centerY, centerX + axisLength, centerY);
    painter.drawText(centerX + axisLength + 5, centerY, "x");

    // Ось Y (зеленая)
    painter.setPen(QPen(Qt::green, 2));
    painter.drawLine(centerX, centerY, centerX, centerY - axisLength);
    painter.drawText(centerX, centerY - axisLength - 5, "y");

    // Ось Z (синяя)
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawLine(centerX, centerY, centerX - axisLength / 2, centerY - axisLength / 2);
    painter.drawText(centerX - axisLength / 2 - 15, centerY - axisLength / 2 - 5, "z");

    if (m_hasShape) {
        // Если есть модель вала, рисуем схематичное изображение вала
        painter.setPen(QPen(Qt::blue, 2));

        // Простая схема вала
        int shaftLength = width() / 2;
        int shaftMaxRadius = height() / 6;

        // Рисуем вал как несколько цилиндров разного диаметра
        int startX = width() / 4;
        int startY = height() / 2;

        // Цилиндр 1 (самый маленький)
        int cylinder1Length = shaftLength / 5;
        int cylinder1Radius = shaftMaxRadius / 2;
        QRect cylinder1(startX, startY - cylinder1Radius, cylinder1Length, 2 * cylinder1Radius);
        painter.drawRect(cylinder1);

        // Цилиндр 2 (средний)
        int cylinder2Length = shaftLength / 3;
        int cylinder2Radius = shaftMaxRadius * 3 / 4;
        QRect cylinder2(startX + cylinder1Length, startY - cylinder2Radius, cylinder2Length, 2 * cylinder2Radius);
        painter.drawRect(cylinder2);

        // Цилиндр 3 (самый большой)
        int cylinder3Length = shaftLength / 2;
        int cylinder3Radius = shaftMaxRadius;
        QRect cylinder3(startX + cylinder1Length + cylinder2Length, startY - cylinder3Radius, cylinder3Length, 2 * cylinder3Radius);
        painter.drawRect(cylinder3);

        // Закругляем края
        painter.drawEllipse(startX - cylinder1Radius / 2, startY - cylinder1Radius, cylinder1Radius, 2 * cylinder1Radius);
        painter.drawEllipse(startX + cylinder1Length + cylinder2Length + cylinder3Length - cylinder3Radius / 2,
                            startY - cylinder3Radius, cylinder3Radius, 2 * cylinder3Radius);
    } else {
        // Если модели нет, выводим текст
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 14));
        painter.drawText(rect(), Qt::AlignCenter, "Нажмите кнопку 'Построить' для создания модели вала");
    }
}

// Реализация ShaftMainWindow

ShaftMainWindow::ShaftMainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_builder(0.025, 45.0),
    m_proportions(230.0, 23.0, 27.0) {
    setWindowTitle("ShaftOCCT - Графический интерфейс");
    setMinimumSize(800, 600);

    setupUI();
}

ShaftMainWindow::~ShaftMainWindow() {
    // Деструктор
}

void ShaftMainWindow::setupUI() {
    // Создаем центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Создаем основной вертикальный layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Создаем виджет для отображения вала
    m_shaftWidget = new ShaftWidget(this);
    mainLayout->addWidget(m_shaftWidget, 2); // Соотношение 2:1

    // Создаем группу для параметров
    QGroupBox *paramsGroup = new QGroupBox("Параметры вала", this);
    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsGroup);

    // Добавляем поля для ввода параметров
    QHBoxLayout *lengthLayout = new QHBoxLayout();
    QLabel *lengthLabel = new QLabel("Длина:", this);
    m_lengthEdit = new QLineEdit("230.0", this);
    lengthLayout->addWidget(lengthLabel);
    lengthLayout->addWidget(m_lengthEdit);
    paramsLayout->addLayout(lengthLayout);

    QHBoxLayout *maxDiamLayout = new QHBoxLayout();
    QLabel *maxDiamLabel = new QLabel("Макс. Радиус:", this);
    m_diameterEdit = new QLineEdit("100.0", this);
    maxDiamLayout->addWidget(maxDiamLabel);
    maxDiamLayout->addWidget(m_diameterEdit);
    paramsLayout->addLayout(maxDiamLayout);

    // Добавляем кнопки
    m_buildButton = new QPushButton("Построить", this);
    connect(m_buildButton, &QPushButton::clicked, this, &ShaftMainWindow::buildShaft);
    paramsLayout->addWidget(m_buildButton);

    m_exportButton = new QPushButton("Экспортировать", this);
    connect(m_exportButton, &QPushButton::clicked, this, &ShaftMainWindow::exportShaft);
    m_exportButton->setEnabled(false); // Изначально кнопка экспорта неактивна
    paramsLayout->addWidget(m_exportButton);

    // Добавляем растягивающийся пробел
    paramsLayout->addStretch(1);

    // Добавляем группу параметров в основной layout
    mainLayout->addWidget(paramsGroup, 1); // Соотношение 2:1
}

void ShaftMainWindow::buildShaft() {
    try {
        // Получаем параметры из полей ввода
        double length = m_lengthEdit->text().toDouble();
        double maxDiameter = m_diameterEdit->text().toDouble();

        if (length <= 0 || maxDiameter <= 0) {
            QMessageBox::warning(this, "Ошибка", "Длина и диаметр должны быть положительными числами");
            return;
        }

        // Устанавливаем параметры вала
        m_proportions.setTotalLength(length);

        // Устанавливаем диаметр для 4-го и 9-го цилиндров пропорционально
        double cylinder4Diameter = 23.0 * (maxDiameter / 100.0);
        double cylinder9Diameter = 27.0 * (maxDiameter / 100.0);
        m_proportions.setCustomDiameter(3, cylinder4Diameter);
        m_proportions.setCustomDiameter(8, cylinder9Diameter);

        // Строим вал
        m_builder.buildFromProportions(m_proportions);
        m_builder.build();

        // Устанавливаем модель для отображения
        m_shaftWidget->setShape(m_builder.getFinalShape());

        // Активируем кнопку экспорта
        m_exportButton->setEnabled(true);

        QMessageBox::information(this, "Успех", "Модель вала успешно построена");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при построении вала: %1").arg(e.what()));
    }
}

void ShaftMainWindow::exportShaft() {
    QString fileName = QFileDialog::getSaveFileName(this, "Экспортировать модель",
                                                    "shaft.step", "STEP Files (*.step);;All Files (*)");
    if (!fileName.isEmpty()) {
        if (m_builder.exportToSTEP(fileName.toStdString())) {
            QMessageBox::information(this, "Успех", "Модель успешно экспортирована в " + fileName);
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось экспортировать модель");
        }
    }
}
