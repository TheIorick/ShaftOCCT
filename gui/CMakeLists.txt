# Создаем GUI приложение
add_executable(Gui WIN32  # WIN32 флаг для Windows, чтобы избежать консольного окна
    mainwindow.cpp
    mainwindow.h
    main.cpp
)

# Подключаем библиотеку Lib
target_link_libraries(Gui PRIVATE Lib)

# Линковка с Qt
target_link_libraries(Gui PRIVATE Qt6::Core Qt6::Widgets)

# Подключаем заголовочные файлы OpenCASCADE (через Lib)
# (Это уже сделано через Lib, но для надежности можно проверить пути)
target_include_directories(Gui PRIVATE ${OCC_INCLUDE_DIR})

# Копирование DLL OpenCASCADE в выходную папку
add_custom_command(TARGET Gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OCC_BIN_DIR}"
    $<TARGET_FILE_DIR:Gui>
)
