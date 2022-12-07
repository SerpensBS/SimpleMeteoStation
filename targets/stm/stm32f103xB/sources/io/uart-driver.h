#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_

#include "cmsis/stm/stm32f1xx.h"
#include "core/rcc-driver.h"
#include "middleware/enums/return-codes.h"

namespace STM32F103XB
{
	/**
	 * Данные о адресах регистра GPIO, с помощью которых конфигурируется USART.
	 */
	struct GPIOAddressesUSART
	{
		volatile uint32_t& gpio_port_register;
		uint32_t tx_cnf_msk;
		int32_t tx_cnf_pos;
		uint32_t tx_mode_msk;
		int32_t tx_mode_pos;
		uint32_t rx_cnf_msk;
		int32_t rx_cnf_pos;
		uint32_t rx_mode_msk;
	};

	/**
	 * Драйвер UART.
	 */
	class UARTDriver
	{
	 private:
		/**
		 * Экземпляр драйвера UART3.
		 */
		static UARTDriver* uart3;

		/**
		 * Инициализация драйвера UART.
		 * @param bus_speed Скорость шины APB, от которой тактируется UART
		 * @param baud_rate UART BaudRate
		 * @param gpio_addresses Адреса регистра GPIO, с помощью которых конфигурируется USART
		 * @param usart_register Адрес регистра USART
		 */
		void Init(
			uint32_t bus_speed,
			uint32_t baud_rate,
			GPIOAddressesUSART& gpio_addresses,
			USART_TypeDef& usart_register);
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера UART, если он не был создан ранее.
		 * @param bus_speed Скорость шины APB, от которой тактируется UART
		 * @param baud_rate UART BaudRate
		 * @param usart_type Адрес регистра USART, который будем настраивать
		 * @param out_uart_driver Драйвер UART
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(
			uint32_t bus_speed,
			uint32_t baud_rate,
			USART_TypeDef& usart_type,
			UARTDriver*& out_uart_driver);
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_
