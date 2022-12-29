#include "i2c-driver.h"
#include "config/device-config.h"

namespace STM32F103XB
{
	I2CDriver I2CDriver::i2c1(I2CDriver(*I2C1));

	void I2CDriver::Init(I2C_TypeDef& i2c_register, RCCDriver& rcc_driver)
	{
		// Включаем тактирование I2C.
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

		// Отключаем I2C.
		i2c_register.CR1 &= I2C_CR1_PE_Msk;

		uint32_t apb1_clock_hz = rcc_driver.GetAPB1Clock();
		uint32_t apb1_clock_mhz = apb1_clock_hz / 1000000UL;

		// Указываем частоту шины APB1.
		i2c_register.CR2 = apb1_clock_mhz << I2C_CR2_FREQ_Pos;

		// Определяем время нарастания фронта.
		i2c_register.TRISE = apb1_clock_mhz + 1;

		/**
		 * Формула имеет вид Tscl = Thigh + Tlow.
		 * Значения Thigh и Tlow можно найти в RM 26.6.8 I2C Clock control register (I2C_CCR).
		 * Значение Tsc можно найти в документе UM10204 табл. 10, стр 44.
		 */
		i2c_register.CCR = static_cast<uint32_t>(0.000005 * static_cast<float>(apb1_clock_hz));

		// Включаем I2C.
		i2c_register.CR1 |= (1 << I2C_CR1_PE_Pos);
	}

	Middleware::ReturnCode I2CDriver::ConfigureGPIO(
		GPIODriver& gpio_driver,
		GPIOPinConfiguration& scl_config,
		GPIOPinConfiguration& sda_config)
	{
		// Настраиваем тактирующий вывод шины I2C.
		auto status = gpio_driver.ConfigurePin(scl_config);

		if (Middleware::ReturnCode::OK != status)
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Настраиваем вывод данных шины I2C.
		status = gpio_driver.ConfigurePin(sda_config);

		if (Middleware::ReturnCode::OK != status)
		{
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode I2CDriver::CreateSingleInstance(I2C_TypeDef& i2c_type,
		GPIODriver& gpio_driver,
		RCCDriver& rccDriver,
		SystemTimer& timer,
		Application::Logger& logger,
		I2CDriver*& out_i2c_driver)
	{
		// Определяем, какой I2C нам нужен, и контролируем уникальность экземпляра драйвера.
		if (&i2c_type == I2C1 && !i2c1.isInitialized)
		{
			// Включаем тактирование порта B и его альтернативные функции.
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

			// Настраиваем GPIO:
			GPIOPinConfiguration i2c_scl_config = {*GPIOB, 6, AlternateFunctionOpenDrain, OutputMaxSpeed2MHz};
			GPIOPinConfiguration i2c_sda_config = {*GPIOB, 7, AlternateFunctionOpenDrain, OutputMaxSpeed2MHz};
			auto status = ConfigureGPIO(gpio_driver, i2c_scl_config, i2c_sda_config);

			if (Middleware::ReturnCode::OK != status)
			{
				logger.Log(Application::LogLevel::Error, "%s", "I2C Initialization: GPIO configure error.");
				return Middleware::ReturnCode::ERROR;
			}

			// Инициализация I2C.
			Init(i2c1.i2c_register_, rccDriver);
			i2c1.isInitialized = true;

			i2c1.timer_ = &timer;
			i2c1.logger_ = &logger;

			out_i2c_driver = &i2c1;
			return Middleware::ReturnCode::OK;
		}

		return Middleware::ReturnCode::ERROR;
	}

	/**
	 * Здесь мы очищаем регистры, поэтому этот метод не должен быть const.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-make-member-function-const"
	void I2CDriver::ClearStatusFlags()
	{
		(void)i2c_register_.SR1;
		(void)i2c_register_.SR2;
	}
	#pragma clang diagnostic pop

	I2CStatusCode I2CDriver::Start()
	{
		// Ждем когда освободится шина.
		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR2, I2C_SR2_BUSY_Msk, 0, DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			return I2CStatusCode::BUS_BUSY;
		}

		// Генерируем START.
		i2c_register_.CR1 |= (1 << I2C_CR1_START_Pos);

		// Ждем появления START Condition.
		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR1, I2C_SR1_SB_Msk, I2C_SR1_SB_Msk, DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			// Если условие не наступило - останавливаем генерацию сигнала START.
			i2c_register_.CR1 &= ~I2C_CR1_START_Msk;
			return I2CStatusCode::TIMEOUT;
		}

		return I2CStatusCode::OK;
	}

	void I2CDriver::Stop()
	{
		// Устанавливаем сигнал STOP на шине.
		i2c_register_.CR1 |= (1 << I2C_CR1_STOP_Pos);
		i2c_register_.SR1 &= ~I2C_SR1_AF_Msk;
	}

	I2CStatusCode I2CDriver::RequestHandshake(uint8_t slave_address, I2CDirection direction)
	{
		auto status = Start();
		if (I2CStatusCode::OK != status)
		{
			return status;
		}

		status = SendAddress(slave_address, direction);
		return status;
	}

	I2CStatusCode I2CDriver::ReadByte(uint8_t* out_received_data)
	{
		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR1, I2C_SR1_RXNE_Msk, I2C_SR1_RXNE, DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			return I2CStatusCode::TIMEOUT;
		}

		*out_received_data = static_cast<uint8_t>(i2c_register_.DR);

		return I2CStatusCode::OK;
	}

	I2CStatusCode I2CDriver::SendAddress(uint8_t slave_address, I2CDirection direction)
	{
		// Смещаем адрес на 1 бит влево.
		slave_address = static_cast<uint8_t>(slave_address << 1);

		// Устанавливаем единицу в конце адреса, чтобы обозначить команду на чтение.
		if (I2CDirection::Read == direction)
		{
			++slave_address;
		}

		i2c_register_.DR = slave_address;

		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR1, I2C_SR1_ADDR_Msk, (1 << I2C_SR1_ADDR_Pos), DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			return I2CStatusCode::ADDRESS_NOT_RESPONDING;
		}

		ClearStatusFlags();
		return I2CStatusCode::OK;
	}

	I2CStatusCode I2CDriver::SendByte(uint8_t data)
	{
		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR1, I2C_SR1_TXE_Msk, (1 << I2C_SR1_TXE_Pos), DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			return I2CStatusCode::TIMEOUT;
		}

		i2c_register_.DR = data;

		if (!timer_->WaitingForRegisterMaskSet(
			i2c_register_.SR1, I2C_SR1_BTF_Msk, I2C_SR1_BTF, DeviceConfig::I2CTransactionTimeoutMs, true))
		{
			return I2CStatusCode::TIMEOUT;
		}

		if (i2c_register_.SR1 & I2C_SR1_AF)
		{
			return I2CStatusCode::ACKNOWLEDGE_FAILURE;
		}
		return I2CStatusCode::OK;
	}

	Middleware::ReturnCode I2CDriver::ReadData(
		uint8_t slave_address,
		uint8_t slave_address_register,
		uint8_t* read_buffer,
		uint32_t receive_bytes_count)
	{
		ClearStatusFlags();

		// Генерируем сигнал START на шине и отправляем адрес.
		I2CStatusCode status = RequestHandshake(slave_address, I2CDirection::Write);
		switch(status)
		{
			case I2CStatusCode::OK:
				break;
			case I2CStatusCode::BUS_BUSY:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Start: Line is busy.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::TIMEOUT:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Start: failed to set Start condition.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::ADDRESS_NOT_RESPONDING:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address: Address not responding.");
				Stop();
				return Middleware::ReturnCode::ERROR;
			/**
			 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
			 * кодов возврата.
			 */
			#pragma clang diagnostic push
			#pragma ide diagnostic ignored "UnreachableCode"
			default:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Handshake: undefined error.");
				return Middleware::ReturnCode::ERROR;
			#pragma clang diagnostic pop
		}

		// Отправляем регистр, который будем читать.
		status = SendByte(slave_address_register);
		if (I2CStatusCode::OK != status)
		{
			switch(status)
			{
				case I2CStatusCode::TIMEOUT:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register Timeout.");
					break;
				case I2CStatusCode::ACKNOWLEDGE_FAILURE:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register: NACK Received.");
					break;
				/**
				 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
				 * кодов возврата.
				 */
				#pragma clang diagnostic push
				#pragma ide diagnostic ignored "UnreachableCode"
				default:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register: undefined error.");
					break;
				#pragma clang diagnostic pop
			}

			Stop();
			return Middleware::ReturnCode::ERROR;
		}

		Stop();
		// Генерируем повторный сигнал START на шине и снова отправляем адрес.
		status = RequestHandshake(slave_address, I2CDirection::Read);
		switch(status)
		{
			case I2CStatusCode::OK:
				break;
			case I2CStatusCode::BUS_BUSY:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C 2nd Start: Line is busy.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::TIMEOUT:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C 2nd Start: failed to set Start condition.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::ADDRESS_NOT_RESPONDING:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C 2nd Send Address: Address not responding.");
				Stop();
				return Middleware::ReturnCode::ERROR;
			/**
			 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
			 * кодов возврата.
			 */
			#pragma clang diagnostic push
			#pragma ide diagnostic ignored "UnreachableCode"
			default:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C 2nd Handshake: undefined error.");
				return Middleware::ReturnCode::ERROR;
			#pragma clang diagnostic pop
		}

		// Если в очереди больше 2 байт - включаем отправку сигнала ACK.
		if (receive_bytes_count > 2)
		{
			i2c_register_.CR1 |= I2C_CR1_ACK;
		}

		for (uint32_t i = 0; i < receive_bytes_count; ++i)
		{
			uint32_t bytes_remain = receive_bytes_count - i;

			status = ReadByte(&read_buffer[i]);
			if (I2CStatusCode::OK != status)
			{
				switch(status)
				{
					case I2CStatusCode::TIMEOUT:
						logger_->Log(Application::LogLevel::Error,
							"I2C Read Data Timeout. Byte %d/%d.", i, receive_bytes_count);
						break;
					/**
				 	* Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
				 	* кодов возврата.
					 */
					#pragma clang diagnostic push
					#pragma ide diagnostic ignored "UnreachableCode"
					default:
						logger_->Log(Application::LogLevel::Error,
							"I2C Read Data: undefined error. Byte %d/%d.", i, receive_bytes_count);
						break;
					#pragma clang diagnostic pop
				}

				// В случае ошибки выходим из цикла.
				break;
			}

			// Когда прочитали предпоследний байт, отключаем отправку ACK и передаем на шину RESTART.
			if (bytes_remain == 2)
			{
				i2c_register_.CR1 &= ~I2C_CR1_ACK_Msk;
				Stop();
				Start();
			}
		}

		Stop();
		return status == I2CStatusCode::OK
			? Middleware::ReturnCode::OK
			: Middleware::ReturnCode::ERROR;
	}

	Middleware::ReturnCode I2CDriver::SendData(
		uint8_t slave_address,
		uint8_t slave_address_register,
		const uint8_t* data_buffer,
		uint32_t data_buffer_length)
	{
		// Генерируем сигнал START на шине и отправляем адрес.
		I2CStatusCode status = RequestHandshake(slave_address, I2CDirection::Write);
		switch(status)
		{
			case I2CStatusCode::OK:
				break;
			case I2CStatusCode::BUS_BUSY:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Start: Line is busy.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::TIMEOUT:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Start: failed to set Start condition.");
				return Middleware::ReturnCode::ERROR;
			case I2CStatusCode::ADDRESS_NOT_RESPONDING:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address: Address not responding.");
				Stop();
				return Middleware::ReturnCode::ERROR;
				/**
				 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
				 * кодов возврата.
				 */
				#pragma clang diagnostic push
				#pragma ide diagnostic ignored "UnreachableCode"
			default:
				logger_->Log(Application::LogLevel::Error, "%s", "I2C Handshake: undefined error.");
				return Middleware::ReturnCode::ERROR;
				#pragma clang diagnostic pop
		}

		// Отправляем регистр, в который будем писать.
		status = SendByte(slave_address_register);
		if (I2CStatusCode::OK != status)
		{
			switch(status)
			{
				case I2CStatusCode::TIMEOUT:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register Timeout.");
					break;
				case I2CStatusCode::ACKNOWLEDGE_FAILURE:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register: NACK Received.");
					break;
					/**
					 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
					 * кодов возврата.
					 */
					#pragma clang diagnostic push
					#pragma ide diagnostic ignored "UnreachableCode"
				default:
					logger_->Log(Application::LogLevel::Error, "%s", "I2C Send Address Register: undefined error.");
					break;
					#pragma clang diagnostic pop
			}

			Stop();
			return Middleware::ReturnCode::ERROR;
		}

		// Начинаем запись.
		for (uint32_t i = 0; i < data_buffer_length; ++i)
		{
			status = SendByte(data_buffer[i]);
			if (I2CStatusCode::OK != status)
			{
				switch(status)
				{
					case I2CStatusCode::TIMEOUT:
						logger_->Log(Application::LogLevel::Error,
							"I2C Send Data Timeout. Byte %d/%d.", i, data_buffer_length);
						break;
					case I2CStatusCode::ACKNOWLEDGE_FAILURE:
						logger_->Log(Application::LogLevel::Error,
							"I2C Send Data Acknowledge Failure. Byte %d/%d.", i, data_buffer_length);
						break;
						/**
						 * Всегда заполняем блок default, чтобы избежать ошибок при увеличении количества возможных проверяемых
						 * кодов возврата.
						 */
						#pragma clang diagnostic push
						#pragma ide diagnostic ignored "UnreachableCode"
					default:
						logger_->Log(Application::LogLevel::Error,
							"I2C Send Data Error: undefined error. Byte %d/%d.", i, data_buffer_length);
						break;
						#pragma clang diagnostic pop
				}

				// В случае ошибки выходим из цикла.
				break;
			}
		}

		Stop();
		return status == I2CStatusCode::OK
			   ? Middleware::ReturnCode::OK
			   : Middleware::ReturnCode::ERROR;
	}
}