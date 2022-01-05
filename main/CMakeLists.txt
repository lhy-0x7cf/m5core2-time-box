file(GLOB MAIN *.c *.cpp)
file(GLOB MAIN_GUI gui/*.c gui/*.cpp)

idf_component_register(
    SRCS ${MAIN_GUI} ${MAIN}
    INCLUDE_DIRS ""
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_LVGL_H_INCLUDE_SIMPLE")
target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_HOR_RES_MAX=320")
target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLV_VER_RES_MAX=240")
