project(BMP2_SENSOR_API C)

file(GLOB SOURCE_FILES bmp2-sensor-api/*.c)
file(GLOB HEADER_FILES bmp2-sensor-api/*.h)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

# Шарим корневую директорию.
target_include_directories(${PROJECT_NAME} PUBLIC .)
# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})