#include "application/core/core.h"
#include "config/device-config.h"
#include "core/rcc-driver.h"
#include "io/uart-driver.h"

int main()
{
	// Инициализация тактирования.
	STM32F103XB::RCCDriver* rcc_driver = nullptr;
	auto status = STM32F103XB::RCCDriver::CreateSingleInstance(STM32F103XB::DeviceConfig::TargetHCLK, rcc_driver);

	if (Middleware::ReturnCode::OK < status || !rcc_driver)
	{
		return 1;
	}

	// Инициализация GPIO.
	STM32F103XB::GPIODriver* gpio_driver = nullptr;
	status = STM32F103XB::GPIODriver::CreateSingleInstance(gpio_driver);

	if (Middleware::ReturnCode::OK < status || !gpio_driver)
	{
		return 1;
	}

	// Инициализация DMA.
	STM32F103XB::DMADriver* dma_driver_channel2 = nullptr;
	status = STM32F103XB::DMADriver::CreateSingleInstance(*DMA1_Channel2, dma_driver_channel2);

	if (Middleware::ReturnCode::OK < status || !dma_driver_channel2)
	{
		return 1;
	}

	// Инициализация UART3 для связи с ПК.
	STM32F103XB::UARTDriver* uart_driver = nullptr;
	status = STM32F103XB::UARTDriver::CreateSingleInstance(
		*USART3,
		*gpio_driver,
		*dma_driver_channel2,
		rcc_driver->GetAPB2Clock(),
		STM32F103XB::DeviceConfig::UART3BaudRate,
		uart_driver);

	if (Middleware::ReturnCode::OK < status || !uart_driver)
	{
		return 1;
	}

	// Запускаем основную логику приложения.
	Application::Core::Run(nullptr, nullptr, nullptr, nullptr, nullptr);
}

