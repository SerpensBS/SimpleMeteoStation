project(Middleware)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE INLINE_FILES *.inl)

add_library(${PROJECT_NAME} INTERFACE ${INLINE_FILES} ${HEADER_FILES})

# Шарим директорию c публичными header'ами.
target_include_directories(${PROJECT_NAME} INTERFACE includes)

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} INTERFACE ${TARGET_COMPILE_OPTIONS})