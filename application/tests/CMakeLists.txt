project(ApplicationTests CXX)

set(APPLICATION_FOLDER ../)

file (GLOB_RECURSE SOURCE_FILES ${APPLICATION_FOLDER}/*.cpp)
file (GLOB_RECURSE HEADER_FILES ${APPLICATION_FOLDER}/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Шарим внутри библиотеки корневую директорию c приватными header'ами.
target_include_directories(${PROJECT_NAME} PRIVATE ${APPLICATION_FOLDER}/includes/application)

# Качаем GTest для Unit-тестирования.
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Подключаем библиотеки.
target_link_libraries(${PROJECT_NAME}
        PRIVATE Middleware
        PRIVATE gtest_main
        )

target_include_directories(${PROJECT_NAME} PRIVATE . ${APPLICATION_FOLDER}/.)

# Подключаем gtest и ищем тесты в проекте.
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})