project(CMSIS C)

file(GLOB_RECURSE SOURCE_FILES *.c)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Шарим директорию c публичными header'ами.
target_include_directories(${PROJECT_NAME} PUBLIC .)

# Шарим внутри все директории, чтобы не править исходники CMSIS.
target_include_directories(${PROJECT_NAME} PRIVATE
        cmsis/arm
        cmsis/stm)

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})