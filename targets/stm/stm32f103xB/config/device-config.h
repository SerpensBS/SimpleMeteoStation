#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_

#include "cmsis/stm/stm32f1xx.h"

namespace STM32F103XB
{
	/**
	 * Конфигурация устройства.
	 */
	struct DeviceConfig
	{
		/**
		 * Тактовая частота, на которую будет настроена AHB.
		 */
		static constexpr uint32_t TargetHCLK = 125000UL;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
