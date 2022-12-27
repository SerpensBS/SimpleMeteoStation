#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_CONSTANTS_GPIO_CONSTANTS_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_CONSTANTS_GPIO_CONSTANTS_H_

#include "cmsis/stm/stm32f1xx.h"

namespace STM32F103XB
{
	/**
	 * Константы GPIO.
	 */
	struct GPIOConstants
	{
		/**
		 * Максимально возможный номер pin'а в порту - 15.
		 */
		static constexpr uint32_t MaxPinNumber = 15;

		/**
		 * Максимальное количество pin'ов в одном конфигурационном регистре CRL или CRH.
		 */
		static constexpr uint32_t MaxPinsInConfigurationRegister = 8;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_CONSTANTS_GPIO_CONSTANTS_H_
