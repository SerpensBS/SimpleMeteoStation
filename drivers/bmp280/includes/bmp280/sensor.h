#ifndef SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_SENSOR_H_
#define SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_SENSOR_H_

#include "application/data/abstract/logger.h"
#include "application/interfaces/isleep.h"
#include "bmp2-sensor-api/bmp2.h"
#include "bmp280/abstract/interfaces/icommunication-bus.h"
#include "bmp280/data/measurement-data.h"
#include "bmp280/data/power-mode.h"
#include "middleware/data/enums/return-codes.h"


namespace BMP280
{
	/**
	 * Драйвер сенсора температуры и давления.
	 */
	class SensorDriver
	{
	 private:
		/**
		 * Структура BMP2 API.
		 */
		bmp2_dev device_ {};

		/**
		 * Конфигурация API.
		 */
		bmp2_config configuration_ {};

		/**
		 * Интерфейс драйвера шины.
		 */
		ICommunicationBus& bus_driver_;

		/**
		 * Интерфейс драйвера контроллера сна.
		 */
		Application::ISleep& sleep_driver_;

		/**
		 * Адрес устройства на шине.
		 */
		uint8_t device_address_ = 0x00;

		/**
		 * Время выполнения измерения в мс.
		 */
		uint32_t measured_time_ms = 0x00;

		/**
		 * Logger.
		 */
		Application::Logger& logger_;

		/**
		 * Метод для подстановки в библиотеку BMP2 для чтения из шины.
		 * @param reg_addr Адрес, из которого читаем данные
		 * @param reg_data_out Буфер для данных, в который будем записывать прочитанные данные
		 * @param length Количество байт данных
		 * @param interface_ptr Интерфейс для чтения данных
		 * @return Статус операции
		 */
		static BMP2_INTF_RET_TYPE ReadData(uint8_t reg_addr, uint8_t* reg_data_out, uint32_t length, void* interface_ptr);

		/**
		 * Метод для подстановки в библиотеку BMP2 для записи в шину.
		 * @param reg_addr Адрес, в который будем писать данные
		 * @param reg_data Буфер для данных, которые будем записывать в шину
		 * @param length Количество байт данных
		 * @param interface_ptr Интерфейс для записи данных
		 * @return Статус операции
		 */
		static BMP2_INTF_RET_TYPE WriteData(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* interface_ptr);

		/**
		 * Метод для подстановки в библиотеку BMP2 для реализации задержки.
		 * Нигде не используется, поэтому просто заглушка.
		 * @param period Время задержки
		 * @param interface_ptr Интерфейс для реализации задержки
		 */
		static void DelayPlug(uint32_t period, void* interface_ptr);

		/**
		 * Рассчитывает время измерений для датчика.
		 * @return Статус операции
		 */
		Middleware::ReturnCode ComputeMeasurementTime();
	 public:
		/**
		 * Инициализация датчика.
		 * @param power_mode Режим работы. Если указан Undefined - режим выставляться не будет.
		 * @return Статус операции
		 */
		Middleware::ReturnCode Initialization(PowerMode power_mode = PowerMode::Undefined);

		/**
		 * Получить данные последнего измерения.
		 * @param measurement_data Данные о температуре и давлении
		 * @return Статус операции
		 */
		Middleware::ReturnCode GetMeasureData(MeasurementData& measurement_data);

		[[nodiscard]]
		uint32_t GetMeasureTimeMs() const;

		/**
		 * Установить режим работы.
		 * @param power_mode Режим работы датчика
		 * @return Статус операции
		 */
		Middleware::ReturnCode SetPowerMode(PowerMode power_mode);

		explicit SensorDriver(
			ICommunicationBus& bus_driver,
			bmp2_intf bus_type,
			Application::ISleep& sleep_driver,
			uint8_t device_address,
			Application::Logger& logger)
		: bus_driver_(bus_driver), sleep_driver_(sleep_driver), device_address_(device_address), logger_(logger)
		{
			device_.intf = bus_type;
			device_.intf_ptr = this;
			device_.read = ReadData;
			device_.write = WriteData;
			device_.delay_us = DelayPlug;
		}

		~SensorDriver() = default;
	};
}

#endif //SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_SENSOR_H_
