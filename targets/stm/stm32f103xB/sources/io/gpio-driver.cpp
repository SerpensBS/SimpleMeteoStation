#include "gpio-driver.h"

namespace STM32F103XB
{
	GPIODriver GPIODriver::instance;

	Middleware::ReturnCode GPIODriver::CreateSingleInstance(GPIODriver*& out_gpio_driver)
	{
		// Контролируем уникальность экземпляра драйвера GPIO.
		if (instance.isInitialized)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.isInitialized = true;
		out_gpio_driver = &instance;
		return Middleware::ReturnCode::OK;
	}

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	Middleware::ReturnCode GPIODriver::ConfigurePin(GPIOPinConfiguration& configuration)
	{
		// Если пытаемся указать номер пина больше максимально возможного - выходим.
		if (configuration.pin_number > GPIOConstants::MaxPinNumber)
		{
			return Middleware::ReturnCode::ERROR;
		}

		// В одном регистре у нас хранится информация о 8 pin'ах, на основе этого определяем, Low или High:
		volatile uint32_t& gpio_port_register = configuration.pin_number < GPIOConstants::MaxPinsInConfigurationRegister
				? configuration.gpio_port.CRL
				: configuration.gpio_port.CRH;

		// Смещаемся под CRH, если необходимо.
		uint32_t pin_number = gpio_port_register == configuration.gpio_port.CRL
				? configuration.pin_number
				: configuration.pin_number - GPIOConstants::MaxPinsInConfigurationRegister;

		// Определяем адреса для CNF и MODE.
		uint32_t cnf_position = GPIO_CRL_CNF0_Pos + ((GPIO_CRL_CNF1_Pos - GPIO_CRL_CNF0_Pos) * pin_number);
		uint32_t mode_position = GPIO_CRL_MODE0_Pos + ((GPIO_CRL_MODE1_Pos - GPIO_CRL_MODE0_Pos) * pin_number);

		// Настраиваем CNF:
		gpio_port_register &= ~(0x03UL << cnf_position);
		gpio_port_register |= configuration.pin_configuration << cnf_position;

		// Настраиваем MODE:
		gpio_port_register &= ~(0x03UL << mode_position);
		gpio_port_register |= configuration.pin_mode << mode_position;

		return Middleware::ReturnCode::OK;
	}
	#pragma clang diagnostic pop
}

