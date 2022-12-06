# Путь к файлам tolchain'а.
set(TOOLCHAIN_PATH /usr/bin/)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}size)

# Уровень оптимизации компилятора.
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(COMPILER_OPTIMIZATION -Og)
else()
    set(COMPILER_OPTIMIZATION -O2)
endif()

# Настройки компиляции.
set(TARGET_COMPILE_OPTIONS
        ${COMPILER_OPTIMIZATION}    # Уровень оптимизации.
        -Wall                       # Показывать все предупреждения.
        -Wextra
        -Werror
        -pedantic
        -fstack-usage               # Cгенерировать файл, в котором указан максимальный объем используемого стека
                                    # для каждой функции.
        -g                          # Добавить отладочную информацию.
                                    # Следующие две опции помогают убирать неиспользуемый код если включен
                                    # параметр --gc-sections в линкере.
        -ffunction-sections         # Помещает каждую функцию (включая статические) в собственный раздел text.func_name.
        -fdata-sections             # Помещает каждую глобальную и статическую переменную в собственный раздел.
        -fno-exceptions             # Отключаем исключения для экономии ROM и ускорения работы.
        -Wno-unknown-pragmas        # Подавляем ошибку о неизвестных pragma'х.
    $<$<COMPILE_LANGUAGE:ASM>:
        -c                          # Останавливает на стадии ассемблирования, но пропускает компоновку.
        -x assembler-with-cpp>      # Явно указывает язык. Позволяет работать с файлами .s вместо .S
    $<$<COMPILE_LANGUAGE:C>:
        ${CMAKE_C_FLAGS}
        -std=gnu11>                 # Версия C.
    $<$<COMPILE_LANGUAGE:CXX>:
        ${CMAKE_CXX_FLAGS}
        -std=gnu++17                # Версия C++.
        -fuse-cxa-atexit            # Поддержка __cxa_atexit для статических деструкторов.
        -Woverloaded-virtual        # Предупреждает о попытке дочернего класса перегрузить виртуальную функцию родителя.
        -fno-rtti                   # Отключает поддержку исключений и динамическую идентификацию типов данных.
    >)