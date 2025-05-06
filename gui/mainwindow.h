#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "../lib/ShaftAppCore.h"

class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onBuildButtonClicked();

private:
    ShaftAppCore core;
    QLineEdit *totalLengthEdit;
    QLineEdit *cylinder4DiameterEdit;
    QLineEdit *cylinder9DiameterEdit;
    QPushButton *buildButton;
};

#endif // MAIN_WINDOW_H
