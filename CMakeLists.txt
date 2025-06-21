cmake_minimum_required(VERSION 3.16)

# Инициализация переменных среды MSVC
if(MSVC)
    # Установка переменных среды для MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    
    # Установка путей к Windows SDK
    set(CMAKE_VS_PLATFORM_TOOLSET "v143")
    set(CMAKE_VS_PLATFORM_NAME "x64")
    
    # Установка генератора Visual Studio
    set(CMAKE_GENERATOR "Visual Studio 17 2022")
    set(CMAKE_GENERATOR_PLATFORM "x64")
endif()

# Явно указываем компилятор MSVC
set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")

project(ShaftOCCT LANGUAGES CXX)

# Включаем автоматическую обработку MOC, UIC, RCC для Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к Qt6
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/msvc2022_64")

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
