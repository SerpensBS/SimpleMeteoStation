#ifndef SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_MEASUREMENT_DATA_H_
#define SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_MEASUREMENT_DATA_H_

#include <cstdint>

namespace BMP280
{
	/**
	 * Данные измерений датчика.
	 */
	struct MeasurementData
	{
		/**
		 * Температура в градусах Цельсия.
		 */
		double Temperature_C = INT32_MIN;

		/**
		 * Давление в hPa.
		 */
		double Pressure_hPa = INT32_MIN;
	};
}

#endif //SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_MEASUREMENT_DATA_H_
