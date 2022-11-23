# Из-за того, что мы собираем проект под микроконтроллер, а не под хост, нужно сообщить об этом CMAKE.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

cmake_minimum_required(VERSION 3.23)
project(SimpleMeteoStation)

# Определяем платформу, под которую будет идти компиляция.
if(${CMAKE_TOOLCHAIN_FILE} MATCHES "arm-none-eabi-gcc-toolchain.cmake")
    set(PLATFORM_NAME "STM32")
elseif (${CMAKE_TOOLCHAIN_FILE} MATCHES "linux-x86-gcc-toolchain.cmake")
    set(PLATFORM_NAME "Linux86")
endif()

if(PLATFORM_NAME)
    # Директория для build-файлов.
    set(OUTPUT_DIRECTORY_PATH "${CMAKE_SOURCE_DIR}/build/${PLATFORM_NAME}/${CMAKE_BUILD_TYPE}")
    # Создаем директорию для build-файлов.
    file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY_PATH})
    # Установка директории для build-файлов.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_PATH}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_PATH}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_PATH}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY_PATH}")

    # Общие библиотеки.
    add_subdirectory(middleware)
    add_subdirectory(application)

    # Определяем конечную цель в зависимости от целевой платформы.
    if(${PLATFORM_NAME} STREQUAL "Linux86")
    elseif (${PLATFORM_NAME} STREQUAL "STM32")
        add_subdirectory(libs/cmsis)
        add_subdirectory(targets/stm/stm32f103xB)
    endif()
endif()