#include "sensor-adapter.h"

namespace STM32F103XB
{
	Middleware::ReturnCode SensorAdapter::GetMeasureData(double* data)
	{
		BMP280::MeasurementData measurement_data = {};
		auto status = driver_.GetMeasureData(measurement_data);

		if (Middleware::ReturnCode::OK != status)
		{
			return Middleware::ReturnCode::ERROR;
		}

		switch (type_)
		{
			case SensorType::Temperature:
				*data = measurement_data.Temperature_C;
				break;
			case SensorType::AtmospherePressure:
				*data = measurement_data.Pressure_hPa;
				break;
			default:
				return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}
}