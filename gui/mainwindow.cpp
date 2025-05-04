#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Пока оставим пустым, позже добавим интерфейс
    setWindowTitle("Shaft Builder GUI");
    resize(400, 300); // Устанавливаем начальный размер окна
}

MainWindow::~MainWindow() {
}
