project(BMP280 CXX)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Шарим директорию c публичными header'ами.
target_include_directories(${PROJECT_NAME} PUBLIC includes)
# Шарим внутри библиотеки основные директории.
target_include_directories(${PROJECT_NAME} PRIVATE .)

# Подключаем библиотеки.
target_link_libraries(${PROJECT_NAME} PUBLIC
        Application
        Middleware
        BMP2_SENSOR_API)

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})