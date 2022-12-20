#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_GPIO_PIN_CONFIGURATION_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_GPIO_PIN_CONFIGURATION_H_

#include "cmsis/stm/stm32f1xx.h"

namespace STM32F103XB
{
	/**
	 * Значения для регистра GPIOx_CR(H/L)_CNF.
	 */
	enum PinConfiguration : uint32_t
	{
		// Input Mode:

		// OutputMode:
		AlternateFunctionPushPull = 0x02,
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_GPIO_PIN_CONFIGURATION_H_
