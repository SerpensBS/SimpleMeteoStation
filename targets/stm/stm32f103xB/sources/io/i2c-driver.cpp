#include "i2c-driver.h"

namespace STM32F103XB
{
	I2CDriver I2CDriver::i2c1(I2CDriver(*I2C1));

	void I2CDriver::Init(I2C_TypeDef& i2c_register, RCCDriver& rcc_driver)
	{
		// Отключаем I2C.
		i2c_register.CR1 &= I2C_CR1_PE_Msk;

		uint32_t apb1_clock_hz = rcc_driver.GetAPB1Clock();
		uint32_t apb1_clock_mhz = apb1_clock_hz / 1000000UL;

		// Указываем частоту шины APB1.
		i2c_register.CR2 = apb1_clock_mhz << I2C_CR2_FREQ_Pos;

		// Определяем время нарастания фронта.
		i2c_register.TRISE = apb1_clock_mhz + 1;

		// Для Slow Mode tLow / tHigh = 2
		i2c_register.CCR = apb1_clock_hz / 100000UL / 2;

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
				logger.Log(Application::LogLevel::Error, "%s", "I2C Initialization Error: GPIO configure error.");
				return Middleware::ReturnCode::ERROR;
			}

			// Включаем I2C.
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

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

}