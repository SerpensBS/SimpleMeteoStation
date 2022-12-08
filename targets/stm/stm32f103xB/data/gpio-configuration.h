#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_GPIO_CONFIGURATION_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_GPIO_CONFIGURATION_H_

#include "cmsis/stm/stm32f1xx.h"
#include "data/enums/gpio/pin-configuration.h"
#include "data/enums/gpio/pin-mode.h"

namespace STM32F103XB
{
	/**
	 * Конфигурация pin'а GPIO.
	 */
	struct GPIOPinConfiguration
	{
		/**
		 * Порт GPIO.
		 */
		GPIO_TypeDef& gpio_port;

		/**
		 * Номер pin'а GPIO.
		 */
		uint32_t pin_number;

		/**
		 * Конфигурация pin'а GPIO.
		 */
		PinConfiguration pin_configuration;

		/**
		 * Режим работы pin'а GPIO.
		 */
		PinMode pin_mode;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_GPIO_CONFIGURATION_H_
