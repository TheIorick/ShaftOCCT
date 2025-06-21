# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Gui LANGUAGES CXX)

# Поиск Qt
find_package(Qt6 COMPONENTS Widgets REQUIRED)


# Включение автоматической обработки moc, uic, rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Исходные файлы
set(GUI_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

# Создание исполняемого файла
add_executable(Gui ${GUI_SOURCES})

# Линковка с библиотекой Lib и Qt
target_link_libraries(Gui PRIVATE Lib Qt::Widgets)


add_custom_command(TARGET Gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OCC_BIN_DIR}"
    $<TARGET_FILE_DIR:Gui>
)
