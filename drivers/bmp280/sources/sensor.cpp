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

	Middleware::ReturnCode SensorDriver::Initialization(PowerMode power_mode)
	{
		// Инициализация датчика и получение регистров калибровочных данных.
		auto status = bmp2_init(&device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Initialization Failure. Code: %d", status);
			bmp2_soft_reset(&device_);
			return Middleware::ReturnCode::ERROR;
		}

		// Обновление конфигурации для последующей настройки.
		status = bmp2_get_config(&configuration_, &device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Get Configuration Failure. Code: %d", status);
			bmp2_soft_reset(&device_);
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
			logger_.Log(Application::LogLevel::Error, "BMP280 Save Configuration Failure. Code: %d", status);
			bmp2_soft_reset(&device_);
			return Middleware::ReturnCode::ERROR;
		}

		if (PowerMode::Undefined != power_mode)
		{
			auto power_mode_set_status = SetPowerMode(BMP280::PowerMode::Forced);
			if (Middleware::ReturnCode::OK!= power_mode_set_status)
			{
				logger_.Log(Application::LogLevel::Error,
					"BMP280 Failed to install PowerMode. Status: %d.", power_mode_set_status);
				bmp2_soft_reset(&device_);
				return Middleware::ReturnCode::ERROR;
			}
		}

		auto compute_measurement_time_status = ComputeMeasurementTime();
		if(Middleware::ReturnCode::OK != compute_measurement_time_status)
		{
			logger_.Log(Application::LogLevel::Error,
				"BMP280 Failed in measured time value calculation. Status: %d.", compute_measurement_time_status);
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SensorDriver::ComputeMeasurementTime()
	{
		uint32_t calculated_value_measured_time_ms = 0;
		int8_t status = bmp2_compute_meas_time(&calculated_value_measured_time_ms, &configuration_, &device_);

		if (BMP2_OK != status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Compute Measurement Time Failure. Code: %d", status);
			return Middleware::ReturnCode::ERROR;
		}

		// Переводим микросекунды в миллисекунды и округляем в большую сторону.
		measured_time_ms = calculated_value_measured_time_ms / 1000 + 1;
		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SensorDriver::GetMeasureData(MeasurementData& measurement_data)
	{
		auto set_power_mode_status = SetPowerMode(PowerMode::Forced);
		if (Middleware::ReturnCode::OK != set_power_mode_status)
		{
			logger_.Log(Application::LogLevel::Error, "BMP280 Get Measurement Data Failure. Code: %d",
				set_power_mode_status);
			return Middleware::ReturnCode::ERROR;
		}

		// TODO: Переписать на более изящное решение, чтобы драйвер уходил в сон на нужное для измерения количество
		//  миллисекунд, а не на целую секунду - это избыточно!
		sleep_driver_.Sleep(1);

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

	uint32_t SensorDriver::GetMeasureTimeMs() const
	{
		return measured_time_ms;
	}

	Middleware::ReturnCode SensorDriver::SetPowerMode(PowerMode power_mode)
	{
		if (PowerMode::Undefined == power_mode)
		{
			logger_.Log(Application::LogLevel::Error, "%s",
				"BMP280 Failed to install PowerMode: estimated status undefined");
			return Middleware::ReturnCode::ERROR;
		}

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

