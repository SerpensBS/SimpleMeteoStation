#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_BMP280_CONFIG_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_BMP280_CONFIG_H_

#include "cmsis/stm/stm32f1xx.h"

namespace STM32F103XB
{
	/**
	 * Конфигурация датчика BMP280.
	 */
	struct BMP280Configuration
	{
		/**
		 * Адрес датчика BMP280 на шине I2C.
		 */
		static constexpr uint8_t Bmp280I2CAddress = 0x76;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_BMP280_CONFIG_H_
