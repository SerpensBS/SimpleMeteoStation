# Простая метеостанция

## Цель проекта

Создать устройство для измерения атмосферных показателей, попрактиковаться в подключении различных датчиков к 
контроллеру и написании объектно-ориентированного кода для как для МК STM32, так и для остальных компонентов устройства.

## Основные требования

- Измерение показателей атмосферы (температура, давление)
- Вывод измеренных показателей на экран устройства

## Компоненты

- Отладочная плата [STM32 Smart V2.0](https://stm32-base.org/boards/STM32F103C8T6-STM32-Smart-V2.0.html#Power-LED)
  - Контроллер [STM32F103C8T6](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)
  - EEPROM [AT24C04](https://ww1.microchip.com/downloads/en/devicedoc/doc0180.pdf)
  - Кварцевый резонатор 8MHz
  - Кварцевый резонатор 32.768KHz
  - Датчик температуры и давления 
    [BMP280](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp280-ds001.pdf)
  - OLED I2C дисплей 128x64 
    [SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)

## Питание

Так как проект учебно-развлекательный, он целиком собирается на макетной плате. Питание на данный момент подается от 
стабилизатора напряжения 3.3V. В будущем планируется перевести его на автономный источник питания.

## Структура проекта

- **application** - платформ-независимая логика приложения - опрос сенсоров, вывод информации на дисплей, управление питанием 
- **libs** - библиотеки, необходимые для работы
- **middleware** - наборы интерфейсов и платформ-независимые общие функции
- **targets** - платформа, обеспечивающая инициализацию, а также доступ к аппаратной части для верхнего уровня
- **toolchains** - здесь перечислены утилиты, необходимые для кросс-компиляции, а также флаги к ним

## Установка

1.  Определяемся с платформой, под которую будем проводить компиляцию. Находим соответствующий тулчейн в папке 
toolchains и настраиваем утилиты/флаги
2.  Открываем конфигурацию нашего target'а по пути targets/XX/XX/config и меняем значения, если это необходимо
3.  Переходим к генерации файлов системой сборки. В параметрах для cmake необходимо указать:
    - **-DCMAKE\_BUILD\_TYPE** - Debug или Release
    - **-DCMAKE\_MAKE\_PROGRAM** -программу сборки, например gmake
    - **-DCMAKE\_C\_COMPILER** - компилятор C. Например /usr/bin/arm-none-eabi-gcc
    - **-DCMAKE\_CXX\_COMPILER** - компилятор C. Например /usr/bin/arm-none-eabi-c++
    - **-DCMAKE\_TOOLCHAIN\_FILE** - файл тулчейна из папки toolchains
    - **-G** - система генерации. Например "Unix Makefile" или "Ninja"
    - **-S** - директория с корневым CMakeLists.txt
    - **-B** - директория, куда будет записан сгенерированный cmake код.
4.  Сборка проекта. В параметрах для cmake необходимо указать:
    - **\-\- build** - директория, которую указали в параметре -B предыдущего шага
    - **\-\- target** - одна из библиотек из папки target, например STM32F103xB
5.  Готовый .hex файл прошивки можно найти по пути, который CMAKE генерирует для этих целей 
по маске SimpleMeteoStation/build/${PLATFORM\_NAME}/${CMAKE\_BUILD_TYPE}. 

### Пример для STM32 Release ###

```
cmake -DCMAKE\_BUILD\_TYPE=Release -DCMAKE\_MAKE\_PROGRAM=gmake -DCMAKE\_C\_COMPILER=/usr/bin/arm-none-eabi-gcc 
-DCMAKE\_CXX\_COMPILER=/usr/bin/arm-none-eabi-c++ -G "Unix Makefiles" 
-DCMAKE\_TOOLCHAIN\_FILE=toolchains/arm-none-eabi-gcc-toolchain.cmake -S SimpleMeteoStation 
-B SimpleMeteoStation/.cmake/stm32/release

cmake --build SimpleMeteoStation/.cmake/stm32/release --target STM32F103xB
```