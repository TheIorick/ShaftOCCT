# Исходные файлы графического приложения
set(SOURCE_FILES
    main.cpp
    ShaftMainWindow.cpp
)

# Заголовочные файлы графического приложения
set(HEADER_FILES
    ShaftMainWindow.h
)

# Создаём графическое приложение
add_executable(Gui ${SOURCE_FILES} ${HEADER_FILES})

# Подключаем библиотеку Lib и Qt6
target_link_libraries(Gui PRIVATE Lib Qt6::Widgets)

# Копируем DLL в выходную папку
add_custom_command(TARGET Gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OCC_BIN_DIR}"
    $<TARGET_FILE_DIR:Gui>
)
