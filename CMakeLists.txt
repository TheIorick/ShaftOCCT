cmake_minimum_required(VERSION 3.16)

project(ShaftOCCT LANGUAGES CXX)

# Включаем автоматическую обработку MOC, UIC, RCC для Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Пути к OpenCASCADE
set(OCC_INSTALL_DIR "E:/programs/occt/install")
set(OCC_INCLUDE_DIR "${OCC_INSTALL_DIR}/inc")
set(OCC_LIB_DIR "${OCC_INSTALL_DIR}/win64/vc14/libd")
set(OCC_BIN_DIR "${OCC_INSTALL_DIR}/win64/vc14/bind")

# Определяем библиотеки OpenCASCADE
set(OCC_LIBRARIES
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

# Находим пакет Qt
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Добавляем подпроекты
add_subdirectory(lib)
add_subdirectory(console)
add_subdirectory(gui)
