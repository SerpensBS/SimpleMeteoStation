#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_I2C_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_I2C_DRIVER_H_

#include "gpio-driver.h"
#include "application/data/abstract/logger.h"
#include "bmp280/abstract/interfaces/icommunication-bus.h"
#include "cmsis/stm/stm32f1xx.h"
#include "data/gpio-configuration.h"
#include "middleware/data/enums/return-codes.h"
#include "sources/core/rcc-driver.h"
#include "sources/data/enums/i2c-direction.h"
#include "sources/data/enums/i2c-status-codes.h"
#include "sources/timers/system-timer.h"

namespace STM32F103XB
{
	/**
	 * Драйвер I2C.
	 */
	class I2CDriver : public BMP280::ICommunicationBus
	{
	 private:
		/**
		 * Драйвер I2C1.
		 */
		static I2CDriver i2c1;

		/**
		 * Регистр I2C.
		 */
		I2C_TypeDef& i2c_register_;

		/**
		 * Таймер.
		 */
		SystemTimer* timer_ = nullptr;

		/**
		 * Logger.
		 */
		Application::Logger* logger_ = nullptr;

		/**
		 * Флаг, предотвращающий повторную инициализацию драйвер I2C.
		 */
		bool isInitialized = false;

		/**
		 * Настройка конфигурации GPIO для I2C.
		 * @param gpio_driver 	Драйвер GPIO
		 * @param scl_config 	Конфигурация тактирующего pin'а.
		 * @param sda_config 	Конфигурация pin'а данных.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode ConfigureGPIO(
			GPIODriver& gpio_driver,
			GPIOPinConfiguration& scl_config,
			GPIOPinConfiguration& sda_config);

		/**
		 * Инициализация I2C.
		 * @param i2c_register 	Регистр I2C
		 * @param rcc_driver	Драйвер RCC
		 */
		static void Init(I2C_TypeDef& i2c_register, RCCDriver& rcc_driver);

		/**
		 * Очистка регистров I2C.SR1 и I2C.SR2.
		 */
		void ClearStatusFlags();

		/**
		 * Генерирует сигнал START на шине.
		 * @return Статус операции
		 */
		I2CStatusCode Start();

		/**
		 * Посылает сигнал STOP на шину.
		 */
		void Stop();

		/**
		 * Посылает сначала сигнал START, а затем отправляет адрес устройства на шину.
		 * @param slave_address Адрес ведомого устройства
		 * @param direction Направление потока (чтение или запись)
		 * @return Статус операции
		 */
		I2CStatusCode RequestHandshake(uint8_t slave_address, I2CDirection direction);

		/**
		 * Прочитать байт из шины
		 * @param out_received_data Адрес байта, куда будет записан результат.
		 * @return Статус операции
		 */
		I2CStatusCode ReadByte(uint8_t* out_received_data);

		/**
		 * Отправить адрес ведомого устройства на шину.
		 * @param slave_address Адрес ведомого устройства
		 * @param direction Направление потока (чтение или запись)
		 * @return Статус операции
		 */
		I2CStatusCode SendAddress(uint8_t slave_address, I2CDirection direction);

		/**
		 * Отправить байт на шину.
		 * @param data Байт данных
		 * @return Статус операции
		 */
		I2CStatusCode SendByte(uint8_t data);

		explicit I2CDriver(I2C_TypeDef& i2c_register)
		: i2c_register_(i2c_register)
		{
		}
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера I2C, если он не был создан ранее.
		 * @param i2c_type Адрес регистра I2C, через который производится настройка.
		 * @param gpio_driver Драйвер GPIO
		 * @param rccDriver	Драйвер RCC
		 * @param timer Таймер
		 * @param logger Logger
		 * @param out_uart_driver Драйвер I2C
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(
			I2C_TypeDef& i2c_type,
			GPIODriver& gpio_driver,
			RCCDriver& rccDriver,
			SystemTimer& timer,
			Application::Logger& logger,
			I2CDriver*& out_i2c_driver);

		Middleware::ReturnCode ReadData(
			uint8_t slave_address,
			uint8_t slave_address_register,
			uint8_t* read_buffer,
			uint32_t receive_bytes_count) override;

		Middleware::ReturnCode SendData(
			uint8_t slave_address,
			uint8_t slave_address_register,
			const uint8_t* data_buffer,
			uint32_t data_buffer_length) override;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_I2C_DRIVER_H_
