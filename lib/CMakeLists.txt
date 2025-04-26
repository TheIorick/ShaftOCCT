# Создаём интерфейсную библиотеку для заголовков
add_library(Lib INTERFACE)

# Указываем заголовочные файлы
target_sources(Lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaftBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaftProportions.h
)

# Указываем пути к заголовкам
target_include_directories(Lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Подключение заголовков OpenCASCADE
target_include_directories(Lib INTERFACE ${OCC_INCLUDE_DIR})

# Линковка библиотек OpenCASCADE (передаём зависимости через INTERFACE)
target_link_libraries(Lib INTERFACE
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
