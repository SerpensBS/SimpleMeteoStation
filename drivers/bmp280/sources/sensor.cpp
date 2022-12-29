#include "bmp280/sensor.h"

namespace BMP280
{
	BMP2_INTF_RET_TYPE SensorDriver::ReadData(uint8_t reg_addr,
		uint8_t* reg_data_out,
		uint32_t length,
		void* interface_ptr)
	{
		auto adapter = reinterpret_cast<SensorDriver*>(interface_ptr);

			auto status = adapter->bus_driver_.ReadData(adapter->device_address_, reg_addr, reg_data_out, length);

			if (Middleware::ReturnCode::OK != status)
			{
				return BMP2_E_COM_FAIL;
			}


		return BMP2_OK;
	}

	BMP2_INTF_RET_TYPE SensorDriver::WriteData(uint8_t reg_addr,
		const uint8_t* reg_data,
		uint32_t length,
		void* interface_ptr)
	{
		auto adapter = reinterpret_cast<SensorDriver*>(interface_ptr);

		auto status = adapter->bus_driver_.SendData(adapter->device_address_, reg_addr, reg_data, length);
		if (Middleware::ReturnCode::OK != status)
		{
			return BMP2_E_COM_FAIL;
		}

		return  BMP2_OK;
	}

	void SensorDriver::DelayPlug(uint32_t period, void* interface_ptr)
	{
		auto adapter = reinterpret_cast<SensorDriver*>(interface_ptr);
		adapter->logger_.Log(Application::LogLevel::Error, "%s", "BMP280 Driver Delay method not implemented!");
	}

	Middleware::ReturnCode SensorDriver::Initialization()
	{
		// Инициализация датчика и получение регистров калибровочных данных.
		auto status = bmp2_init(&device_);

		if (BMP2_OK != status)
		{
			bmp2_soft_reset(&device_);
			logger_.Log(Application::LogLevel::Error, "BMP280 Initialization Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		// Обновление конфигурации для последующей настройки.
		status = bmp2_get_config(&configuration_, &device_);

		if (BMP2_OK != status)
		{
			bmp2_soft_reset(&device_);
			logger_.Log(Application::LogLevel::Error, "BMP280 Get Configuration Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		configuration_.filter = BMP2_FILTER_COEFF_4;
		configuration_.os_temp = BMP2_OS_4X;
		configuration_.os_pres = BMP2_OS_4X;
		configuration_.os_mode = BMP2_OS_MODE_STANDARD_RESOLUTION;

		// Настройка конфигурации.
		status = bmp2_set_config(&configuration_, &device_);

		if (BMP2_OK != status)
		{
			bmp2_soft_reset(&device_);
			logger_.Log(Application::LogLevel::Error, "BMP280 Save Configuration Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SensorDriver::ComputeMeasurementTime(uint32_t& out_measurement_time)
	{
		// Рассчитываем время измерения.
		int8_t status = bmp2_compute_meas_time(&out_measurement_time, &configuration_, &device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Compute Measurement Time Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SensorDriver::GetMeasureData(MeasurementData& measurement_data)
	{
		bmp2_data data {};
		int8_t status = bmp2_get_sensor_data(&data, &device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Get Measurement Data Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		measurement_data.Temperature_C = data.temperature;
		measurement_data.Pressure_hPa = data.pressure;

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SensorDriver::SetPowerMode(PowerMode power_mode)
	{
		// Устанавливаем режим работы датчика.
		int8_t status = bmp2_set_power_mode(static_cast<uint8_t>(power_mode), &configuration_, &device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Set Power Mode Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}
}

