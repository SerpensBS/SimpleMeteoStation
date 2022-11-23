#include "application/core/application.h"
#include "config/device-config.h"
#include "core/rcc-driver.h"

int main()
{
	// Инициализация тактирования.
	STM32F103XB::RCCDriver* rcc_driver = nullptr;
	auto status = STM32F103XB::RCCDriver::CreateSingleInstance(STM32F103XB::DeviceConfig::TargetHCLK, rcc_driver);

	if (Middleware::ReturnCode::OK < status || !rcc_driver)
	{
		return 1;
	}

	// Создаем экземпляр приложения.
	Application::Application* application = nullptr;
	status = Application::Application::CreateSingleInstance(application);

	if (Middleware::ReturnCode::OK < status || !application)
	{
		return 1;
	}

	// Запускаем приложение.
	application->Run();
	return 0;
}