if ("${CMAKE_C_COMPILER}" STREQUAL "" OR "${CMAKE_CXX_COMPILER}" STREQUAL "" )
    return()
endif()

# За директорию, в которой лежат остальные файлы toolchain'а, примем директорию, в которой лежит компилятор C++.
get_filename_component(TOOLCHAIN_DIRECTORY ${CMAKE_CXX_COMPILER} DIRECTORY)
set(TOOLCHAIN_PREFIX ${TOOLCHAIN_DIRECTORY}/arm-none-eabi-)

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CPU_PARAMETERS
        -mcpu=cortex-m3     # Процессорное ядро.
        -mthumb             # Набор инструкция Thumb.
        -mfloat-abi=soft    # Использовать программные инструкции при работе с плавающей точкой.
        )


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(BUILD_TYPE_FLAGS
            -O0                     # Уровень оптимизации компилятора.

            -g                      # Добавить отладочную информацию.
                                    # Следующие две опции помогают убирать неиспользуемый код если включен
                                    # параметр --gc-sections в линкере.
            )
else()
    set(BUILD_TYPE_FLAGS
            -O2                     # Уровень оптимизации компилятора.
            -Werror                 # Все warning'и становятся error'ами.
            )
endif()

# Подключаем общие флаги.
include(${CMAKE_SOURCE_DIR}/toolchains/gcc-toolchains-general-flags.cmake)

# Настройки компиляции.
set(TARGET_COMPILE_OPTIONS
        ${GENERAL_GCC_COMPILE_FLAGS}
        ${CPU_PARAMETERS}
        ${BUILD_TYPE_FLAGS}
        )

# Настройки линковки.
set(TARGET_LINKER_OPTIONS
        ${CPU_PARAMETERS}
        --specs=nosys.specs                                         # Подключения спец версий libc для мк (newlib)
        --specs=nano.specs                                          # без поддержки многопоточности.
        -Wl,--print-memory-usage                                    # Показывает использованную и оставшуюся память.
        -Wl,--gc-sections
        )