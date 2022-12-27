#include "application/core/core.h"
#include "sources/utils/uart-logger.h"
#include "sources/core/power-driver.h"
#include "sources/io/i2c-driver.h"

/**
 * Бесконечный цикл. Нужен для того, чтобы DMA продолжило обрабатывать все запросы после окончания работы приложения.
 */
[[noreturn]] void InfiniteLoop()
{
	while (true)
	{
	}
}

int main()
{
	// Инициализация тактирования.
	STM32F103XB::RCCDriver* rcc_driver = nullptr;
	auto status = STM32F103XB::RCCDriver::CreateSingleInstance(STM32F103XB::DeviceConfig::TargetHCLK, rcc_driver);

	if (Middleware::ReturnCode::OK < status || !rcc_driver)
	{
		return static_cast<int>(status);
	}

	// Инициализация системного таймера.
	STM32F103XB::SystemTimer::Init(*rcc_driver);

	// Инициализация GPIO.
	STM32F103XB::GPIODriver* gpio_driver = nullptr;
	status = STM32F103XB::GPIODriver::CreateSingleInstance(gpio_driver);

	if (Middleware::ReturnCode::OK < status || !gpio_driver)
	{
		return static_cast<int>(status);
	}

	// Инициализация DMA.
	STM32F103XB::DMADriver* dma_driver_channel2 = nullptr;
	status = STM32F103XB::DMADriver::CreateSingleInstance(*DMA1_Channel2, dma_driver_channel2);

	if (Middleware::ReturnCode::OK < status || !dma_driver_channel2)
	{
		return static_cast<int>(status);
	}

	// Инициализация UART3 для связи с ПК.
	STM32F103XB::UARTDriver* uart_driver = nullptr;
	status = STM32F103XB::UARTDriver::CreateSingleInstance(
		*USART3,
		*rcc_driver,
		*gpio_driver,
		*dma_driver_channel2,
		rcc_driver->GetAPB2Clock(),
		STM32F103XB::DeviceConfig::UART3BaudRate,
		uart_driver);

	if (Middleware::ReturnCode::OK < status || !uart_driver)
	{
		return static_cast<int>(status);
	}

	// Настраиваем logger.
	STM32F103XB::UARTLogger logger(
		*uart_driver,
		Application::ApplicationConfiguration::LoggerLevelConfiguration);

	logger.Log(Application::LogLevel::Info, "%s", "RCC Initialization: OK");
	logger.Log(Application::LogLevel::Trace, "AHB Clock:  %lu", SystemCoreClock);
	logger.Log(Application::LogLevel::Trace, "APB1 Clock: %lu", rcc_driver->GetAPB1Clock());
	logger.Log(Application::LogLevel::Trace, "APB2 Clock: %lu\n\r", rcc_driver->GetAPB2Clock());

	logger.Log(Application::LogLevel::Info, "%s", "UART Initialization: OK");
	logger.Log(Application::LogLevel::Trace, "UART3 BaudRate: %lu\n\r", uart_driver->GetCurrentBaudRate());

	// Инициализация драйвера RTC.
	STM32F103XB::SystemTimer rtc_timer;
	STM32F103XB::RTCDriver* rtc_driver = nullptr;
	status = STM32F103XB::RTCDriver::CreateSingleInstance(rtc_timer, logger, rtc_driver, true);

	if (Middleware::ReturnCode::OK < status || !rtc_driver)
	{
		logger.Log(Application::LogLevel::Error, "Failed to initialize RTC driver. Error code:  %d", status);
		InfiniteLoop();
	}

	// Инициализация драйвера управления питанием.
	STM32F103XB::PowerDriver* power_driver = nullptr;
	status = STM32F103XB::PowerDriver::CreateSingleInstance(*dma_driver_channel2, *rtc_driver, logger, power_driver);

	if (Middleware::ReturnCode::OK < status || !power_driver)
	{
		logger.Log(Application::LogLevel::Error, "Failed to initialize power control driver. Error code:  %d", status);
		InfiniteLoop();
	}

	STM32F103XB::SystemTimer i2c_timer;
	STM32F103XB::I2CDriver* i2c_driver = nullptr;
	status = STM32F103XB::I2CDriver::CreateSingleInstance(*I2C1, *gpio_driver, *rcc_driver, i2c_timer, logger, i2c_driver);

	if (Middleware::ReturnCode::OK < status || !i2c_driver)
	{
		logger.Log(Application::LogLevel::Error, "Failed to initialize I2C driver. Error code:  %d", status);
		InfiniteLoop();
	}

	// Запускаем основную логику приложения.
	logger.Log(Application::LogLevel::Info, "%s", "Start Application...");
	status = Application::Core::Run(nullptr, nullptr, power_driver, nullptr, rtc_driver, &logger);

	auto log_level = Middleware::ReturnCode::OK == status
		? Application::LogLevel::Info
		: Application::LogLevel::Error;

	logger.Log(log_level, "Application exit with return code: %d", status);
	InfiniteLoop();
}