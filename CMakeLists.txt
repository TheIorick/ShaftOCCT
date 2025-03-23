
cmake_minimum_required(VERSION 3.16)

project(ShaftOCCT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Пути к OpenCASCADE (оставьте как есть, если они верны)
set(OCC_INSTALL_DIR "E:/programs/occt/install")
set(OCC_INCLUDE_DIR "${OCC_INSTALL_DIR}/inc")
set(OCC_LIB_DIR "${OCC_INSTALL_DIR}/win64/vc14/libd")
set(OCC_BIN_DIR "${OCC_INSTALL_DIR}/win64/vc14/bind")

# Создание исполняемого файла
add_executable(ShaftOCCT
    ShaftApplication.cpp
    ShaftBuilder.h
    ShaftProportions.h)


# Подключение заголовков OpenCASCADE
target_include_directories(ShaftOCCT PRIVATE E:/programs/occt/install/inc)

# Линковка библиотек OpenCASCADE
target_link_libraries(ShaftOCCT
    "E:/programs/occt/install/win64/vc14/libd/TKernel.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKBO.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKMesh.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKDESTEP.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKMath.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKDESTL.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKPrim.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKTopAlgo.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKBRep.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKFillet.lib"
    "E:/programs/occt/install/win64/vc14/libd/TKG3d.lib"    
)

# Копирование DLL в выходную папку (оставьте как есть)
add_custom_command(TARGET ShaftOCCT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OCC_BIN_DIR}"
    $<TARGET_FILE_DIR:ShaftOCCT>
)
