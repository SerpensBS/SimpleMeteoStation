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

# Уровень оптимизации компилятора.
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(COMPILER_OPTIMIZATION -Og)
else()
    set(COMPILER_OPTIMIZATION -O2)
endif()

# Настройки компиляции.
set(TARGET_COMPILE_OPTIONS
        ${CPU_PARAMETERS}
        ${COMPILER_OPTIMIZATION}# Уровень оптимизации.
        -Wall                   # Показывать все предупреждения.
        -fstack-usage           # Cгенерировать файл, в котором указан максимальный объем используемого стека
                                # для каждой функции.
        -g                      # Добавить отладочную информацию.
                                # Следующие две опции помогают убирать неиспользуемый код если включен
                                # параметр --gc-sections в линкере.
        -ffunction-sections     # Помещает каждую функцию (включая статические) в собственный раздел text.func_name.
        -fdata-sections         # Помещает каждую глобальную и статическую переменную в собственный раздел.
        -fno-exceptions         # Отключаем исключения для экономии ROM и ускорения работы.
        -Wno-unknown-pragmas    # Подавляем ошибку о неизвестных pragma'х.

        $<$<COMPILE_LANGUAGE:ASM>:
        -c                      # Останавливает на стадии ассемблирования, но пропускает компоновку.
        -x assembler-with-cpp>  # Явно указывает язык. Позволяет работать с файлами .s вместо .S

        $<$<COMPILE_LANGUAGE:C>:
        ${CMAKE_C_FLAGS}
        -std=gnu11>             # Версия C.

        $<$<COMPILE_LANGUAGE:CXX>:
        ${CMAKE_CXX_FLAGS}
        -std=gnu++17            # Версия C++.
        -fuse-cxa-atexit        # Поддержка __cxa_atexit для статических деструкторов.
        -Woverloaded-virtual    # Предупреждает о попытке дочернего класса перегрузить виртуальную функцию родителя.
        >)

# Настройки линковки.
set(TARGET_LINKER_OPTIONS
        ${CPU_PARAMETERS}
        --specs=nosys.specs                                         # Подключения спец версий libc для мк (newlib)
        --specs=nano.specs                                          # без поддержки многопоточности.
        -Wl,--print-memory-usage                                    # Показывает использованную и оставшуюся память.
        -Wl,--gc-sections
        )