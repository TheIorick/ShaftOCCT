# Создаем консольное приложение
add_executable(Console ShaftApplication.cpp ShaftApplication.h)

# Подключаем библиотеку
target_link_libraries(Console PRIVATE Lib)

# Копирование DLL в выходную папку
add_custom_command(TARGET Console POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OCC_BIN_DIR}"
    $<TARGET_FILE_DIR:Console>
)
