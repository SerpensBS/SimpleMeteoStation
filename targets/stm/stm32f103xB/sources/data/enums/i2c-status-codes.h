#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_ENUMS_I2C_STATUS_CODES_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_ENUMS_I2C_STATUS_CODES_H_

namespace STM32F103XB
{
	/**
	 * Возможные статусы на шине I2C.
	 */
	enum class I2CStatusCode
	{
		OK,
		TIMEOUT,
		ADDRESS_NOT_RESPONDING,
		BUS_BUSY,
		BTF_ERROR,
		ACKNOWLEDGE_FAILURE
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_ENUMS_I2C_STATUS_CODES_H_
