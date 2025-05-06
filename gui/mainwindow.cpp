#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), core(230.0, 23.0, 27.0, 0.025, 45.0) {
    setWindowTitle("Shaft Builder");
    resize(400, 300);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Поле для общей длины
    layout->addWidget(new QLabel("Total Length (mm):"));
    totalLengthEdit = new QLineEdit("230.0", this);
    layout->addWidget(totalLengthEdit);

    // Поле для диаметра 4-го цилиндра
    layout->addWidget(new QLabel("Cylinder 4 Diameter (mm):"));
    cylinder4DiameterEdit = new QLineEdit("23.0", this);
    layout->addWidget(cylinder4DiameterEdit);

    // Поле для диаметра 9-го цилиндра
    layout->addWidget(new QLabel("Cylinder 9 Diameter (mm):"));
    cylinder9DiameterEdit = new QLineEdit("27.0", this);
    layout->addWidget(cylinder9DiameterEdit);

    // Кнопка "Построить"
    buildButton = new QPushButton("Build", this);
    layout->addWidget(buildButton);

    layout->addStretch();

    // Подключение сигнала кнопки
    connect(buildButton, &QPushButton::clicked, this, &MainWindow::onBuildButtonClicked);
}

void MainWindow::onBuildButtonClicked() {
    bool ok;
    double totalLength = totalLengthEdit->text().toDouble(&ok);
    if (!ok || totalLength <= 0) {
        QMessageBox::critical(this, "Error", "Invalid total length. Must be a positive number.");
        return;
    }

    double cylinder4Diameter = cylinder4DiameterEdit->text().toDouble(&ok);
    if (!ok || cylinder4Diameter <= 0) {
        QMessageBox::critical(this, "Error", "Invalid diameter for cylinder 4. Must be a positive number.");
        return;
    }

    double cylinder9Diameter = cylinder9DiameterEdit->text().toDouble(&ok);
    if (!ok || cylinder9Diameter <= 0) {
        QMessageBox::critical(this, "Error", "Invalid diameter for cylinder 9. Must be a positive number.");
        return;
    }

    // Установка параметров
    core.setTotalLength(totalLength);
    core.setSegmentDiameter(4, cylinder4Diameter);
    core.setSegmentDiameter(9, cylinder9Diameter);

    // Запрос пути для сохранения файла
    QString fileName = QFileDialog::getSaveFileName(this, "Save STEP File", "", "STEP Files (*.step *.stp)");
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No file selected.");
        return;
    }

    // Выполнение построения
    int result = core.run(fileName.toStdString());
    if (result == 0) {
        QMessageBox::information(this, "Success", "Shaft built and saved successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to build shaft.");
    }
}
