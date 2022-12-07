if ("${CMAKE_C_COMPILER}" STREQUAL "" OR "${CMAKE_CXX_COMPILER}" STREQUAL "" )
    return()
endif()

# За директорию, в которой лежат остальные файлы toolchain'а, примем директорию, в которой лежит компилятор C++.
get_filename_component(TOOLCHAIN_DIRECTORY ${CMAKE_CXX_COMPILER} DIRECTORY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIRECTORY}/gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIRECTORY}/g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIRECTORY}/objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_DIRECTORY}/objdump)
set(CMAKE_SIZE ${TOOLCHAIN_DIRECTORY}/size)

# Подключаем общие флаги.
include(${CMAKE_SOURCE_DIR}/toolchains/gcc-toolchains-general-flags.cmake)

# Настройки компиляции.
set(TARGET_COMPILE_OPTIONS
        ${GENERAL_GCC_COMPILE_FLAGS}
        -O0                             # Уровень оптимизации.
    )