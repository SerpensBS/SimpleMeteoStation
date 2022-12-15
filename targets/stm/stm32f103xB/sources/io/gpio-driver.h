#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_GPIO_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_GPIO_DRIVER_H_

#include "cmsis/stm/stm32f1xx.h"
#include "data/constants/gpio_constants.h"
#include "data/gpio-configuration.h"
#include "middleware/data/enums/return-codes.h"

namespace STM32F103XB
{
	/**
	 * Драйвер GPIO.
	 */
	class GPIODriver
	{
	 private:
		/**
 		 * Singleton экземпляр GPIO драйвера.
 		 */
		static GPIODriver instance;

		/**
		 * Флаг, предотвращающий повторную инициализацию GPIO.
		 */
		bool isInitialized = false;

		GPIODriver() = default;
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера GPIO, если он не был создан ранее.
		 * @param out_gpio_driver Инициализированный экземпляр драйвера GPIO.
		 * 						  Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 */
		static Middleware::ReturnCode CreateSingleInstance(GPIODriver*& out_gpio_driver);

		/**
		 * Конфигурирует режим работы pin'а
		 * @param configuration Конфигурация pin'а GPIO
		 * @return Статус операции
		 */
		Middleware::ReturnCode ConfigurePin(GPIOPinConfiguration& configuration);
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_GPIO_DRIVER_H_
