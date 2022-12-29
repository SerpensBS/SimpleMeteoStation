#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_ADAPTERS_SENSOR_ADAPTER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_ADAPTERS_SENSOR_ADAPTER_H_

#include "application/interfaces/isensor.h"
#include "bmp280/sensor.h"
#include "sources/data/sensor-type.h"

namespace STM32F103XB
{
	/**
	 * Адаптер для сенсора.
	 */
	class SensorAdapter : public Application::ISensor
	{
	 private:
		/**
		 * Драйвер сенсора.
		 */
		BMP280::SensorDriver& driver_;

		/**
		 * Тип сенсора.
		 */
		SensorType type_;
	 public:
		SensorAdapter(BMP280::SensorDriver& driver, SensorType sensor_type)
		: driver_(driver), type_(sensor_type)
		{
		}

		/**
		 * Получить данные сенсора.
		 * @param data Данные измерений сенсора
		 * @return Статус операции
		 */
		Middleware::ReturnCode GetMeasureData(double* data) override;

		~SensorAdapter() override = default;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_ADAPTERS_SENSOR_ADAPTER_H_
