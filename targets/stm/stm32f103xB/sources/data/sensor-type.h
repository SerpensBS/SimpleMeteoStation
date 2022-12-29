#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_SENSOR_TYPE_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_SENSOR_TYPE_H_

namespace STM32F103XB
{
	/**
	 * Тип сенсора.
	 */
	enum class SensorType
	{
		/**
		 * Температурный сенсор.
		 */
		Temperature,

		/**
		 * Сенсор атмосферного давления.
		 */
		AtmospherePressure
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_DATA_SENSOR_TYPE_H_
