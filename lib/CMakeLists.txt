# Минимальная версия CMake
cmake_minimum_required(VERSION 3.16)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Файлы библиотеки
set(LIB_SOURCES
    ShaftBuilder.h
    ShaftProportions.h
    ShaftAppCore.cpp
    ShaftAppCore.h
)

# Создаём статическую библиотеку
add_library(Lib STATIC ${LIB_SOURCES}
    Slot.h)

# Указываем пути к заголовкам библиотеки
target_include_directories(Lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключение заголовков OpenCASCADE
target_include_directories(Lib PUBLIC
    ${OCC_INCLUDE_DIR}
)

# Линковка библиотек OpenCASCADE
target_link_libraries(Lib PUBLIC
    "${OCC_LIB_DIR}/TKernel.lib"
    "${OCC_LIB_DIR}/TKBO.lib"
    "${OCC_LIB_DIR}/TKMesh.lib"
    "${OCC_LIB_DIR}/TKDESTEP.lib"
    "${OCC_LIB_DIR}/TKMath.lib"
    "${OCC_LIB_DIR}/TKDESTL.lib"
    "${OCC_LIB_DIR}/TKPrim.lib"
    "${OCC_LIB_DIR}/TKTopAlgo.lib"
    "${OCC_LIB_DIR}/TKBRep.lib"
    "${OCC_LIB_DIR}/TKFillet.lib"
    "${OCC_LIB_DIR}/TKG3d.lib"
)
