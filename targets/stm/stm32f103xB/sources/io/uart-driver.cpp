#include "uart-driver.h"

namespace STM32F103XB
{
	UARTDriver* UARTDriver::uart3 = nullptr;

	Middleware::ReturnCode UARTDriver::CreateSingleInstance(
		uint32_t bus_speed,
		uint32_t baud_rate,
		USART_TypeDef& usart_type,
		UARTDriver*& out_uart_driver)
	{
		// Определяем, какой UART нам нужен, и контролируем уникальность экземпляра драйвера.
		if (&usart_type == USART3 && nullptr == uart3)
		{
			static UARTDriver static_uart3_driver;
			*&uart3 = &static_uart3_driver;

			// Включаем тактирование порта B и его альтернативные функции.
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
			// Включаем тактирование UART3.
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

			GPIOAddressesUSART gpio_addresses =
			{
				GPIOB->CRH,
				GPIO_CRH_CNF10_Msk,
				GPIO_CRH_CNF10_Pos,
				GPIO_CRH_MODE10_Msk,
				GPIO_CRH_MODE10_Pos,
				GPIO_CRH_CNF11_Msk,
				GPIO_CRH_CNF11_Pos,
				GPIO_CRH_MODE11_Msk
			};

			uart3->Init(bus_speed, baud_rate, gpio_addresses, *USART3);
			out_uart_driver = uart3;
		}
		else
		{
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	/**
 	* Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
 	* экземпляром класса.
 	*/
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void UARTDriver::Init(
		uint32_t bus_speed,
		uint32_t baud_rate,
		GPIOAddressesUSART& gpio_addresses,
		USART_TypeDef& usart_register)
	{
		// Настраиваем передачу:
		// Alternate Function Push-Pull.
		gpio_addresses.gpio_port_register &= ~gpio_addresses.tx_cnf_msk;
		gpio_addresses.gpio_port_register |= 0x02U << gpio_addresses.tx_cnf_pos;

		// Output mode, 2MHz.
		gpio_addresses.gpio_port_register &= ~gpio_addresses.tx_mode_msk;
		gpio_addresses.gpio_port_register |= 0x02U << gpio_addresses.tx_mode_pos;

		// Настраиваем прием:
		// Input with Pull-up / Pull-down.
		gpio_addresses.gpio_port_register &= ~gpio_addresses.rx_cnf_msk;
		gpio_addresses.gpio_port_register |= 0x01U << gpio_addresses.rx_cnf_pos;

		// Input mode.
		gpio_addresses.gpio_port_register &= ~gpio_addresses.rx_mode_msk;

		// Настраиваем Baud Rate.;
		usart_register.BRR = bus_speed / baud_rate;

		usart_register.CR1 |= USART_CR1_UE	// Включаем тактирование USART.
			| USART_CR1_TCIE        		// Включаем прерывания по Transmission complete.
			| USART_CR1_TE;					// Включаем передачу.

		// Сбрасываем флаг завершения передачи (по-умолчанию выставлен).
		usart_register.SR &= ~USART_SR_TC_Msk;
	}
	#pragma clang diagnostic pop
}