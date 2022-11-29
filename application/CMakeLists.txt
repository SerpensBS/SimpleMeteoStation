project(Application CXX)

file(GLOB_RECURSE SOURCE_FILES sources/*.cpp)
file(GLOB_RECURSE HEADER_FILES includes/*.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Шарим директорию c публичными header'ами.
target_include_directories(${PROJECT_NAME} PUBLIC includes)
# Шарим внутри библиотеки корневую директорию.
target_include_directories(${PROJECT_NAME} PRIVATE . includes/application)

# Подключаем библиотеки.
target_link_libraries(${PROJECT_NAME} PUBLIC Middleware)

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})