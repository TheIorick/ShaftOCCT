/**
 * @file main.cpp
 * @brief Входная точка для графического приложения ShaftOCCT
 */

#include "ShaftMainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Настраиваем формат поверхности для совместимости с OpenGL
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    // Создаем и показываем главное окно
    ShaftMainWindow window;
    window.show();

    return app.exec();
}
