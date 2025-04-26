cmake_minimum_required(VERSION 3.16)

project(ShaftOCCT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути к OpenCASCADE (как у тебя)
set(OCC_INSTALL_DIR "E:/programs/occt/install")
set(OCC_INCLUDE_DIR "${OCC_INSTALL_DIR}/inc")
set(OCC_LIB_DIR "${OCC_INSTALL_DIR}/win64/vc14/libd")
set(OCC_BIN_DIR "${OCC_INSTALL_DIR}/win64/vc14/bind")

# Добавляем подкаталоги
add_subdirectory(Lib)
add_subdirectory(Console)
