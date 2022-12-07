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

	// Инициализация UART3 для связи с ПК.
	STM32F103XB::UARTDriver* uart_driver = nullptr;
	status = STM32F103XB::UARTDriver::CreateSingleInstance(
		rcc_driver->GetAPB2Clock(),
		STM32F103XB::DeviceConfig::UART3_BaudRate,
		*USART3, uart_driver);

	if (Middleware::ReturnCode::OK < status || !uart_driver)
	{
		return 1;
	}

	Application::Core::Run(nullptr, nullptr, nullptr, nullptr, nullptr);
}