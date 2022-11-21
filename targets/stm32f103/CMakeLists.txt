# Название проекта и используемые языки.
project(STM32F103 C CXX ASM)

file (GLOB_RECURSE SOURCE_FILES
        *.cpp)

file (GLOB_RECURSE HEADER_FILES
        *.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Глобальный define для контроллера STM32F103C8T6, нужен для CMSIS.
add_definitions(-DSTM32F103xB)

# Заголовочные файлы CMSIS.
set(CMSIS_INCLUDE_DIRECTORIES
        ${CMSIS_DIRECTORY}/arm
        ${CMSIS_DIRECTORY}/stm/includes)

# Стартовые файлы CMSIS.
set(CMSIS_STARTUP_FILES
        ${CMSIS_DIRECTORY}/stm/system_stm32f1xx.c
        ${CMSIS_DIRECTORY}/stm/startup_stm32f103xb.s)

# Скрипт linker'а.
set(MCU_LINKER_SCRIPT ${CMSIS_DIRECTORY}/stm/STM32F103XB_FLASH.ld)

# Добавляем к исходникам startup файлы из CMSIS.
target_sources(${PROJECT_NAME} PRIVATE ${CMSIS_STARTUP_FILES})

# Подключаем заголовочные файлы CMSIS.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMSIS_INCLUDE_DIRECTORIES})

# Настройки компиляции.
target_compile_options(${PROJECT_NAME} PRIVATE ${TARGET_COMPILE_OPTIONS})

# Настройки линковки.
target_link_options(${PROJECT_NAME} PRIVATE
        -T${MCU_LINKER_SCRIPT}                                      # Подключаем скрипт linker'а.
        -Wl,-Map=${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.map       # Генерируем Map-файл.
        ${TARGET_LINKER_OPTIONS})

# Расширение скомпилированного файла.
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

# Превращаем .elf в .hex
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex
        ${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.elf ${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.hex)
# Превращаем .elf в .lst
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} ARGS --disassemble-all --headers --source -marm
        ${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.elf > ${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.lst)
# Вывод в консоль данных о размере секций .bss, .data и т.д., а так же всего проекта.
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND echo "Sections size:")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_SIZE} ARGS --format=gnu
        ${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.elf)

# Список файлов, которые будут удалены из папки с build-файлами.
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        ADDITIONAL_CLEAN_FILES
        "${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.lst;${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.hex;${OUTPUT_DIRECTORY_PATH}/${PROJECT_NAME}.map")