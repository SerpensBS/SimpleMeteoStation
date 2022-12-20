#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_

#include "cmsis/stm/stm32f1xx.h"
#include "config/device-config.h"
#include "core/rcc-driver.h"
#include "gpio-driver.h"
#include "memory/dma-driver.h"
#include "middleware/collections/circuit-buffer.h"
#include "middleware/data/enums/return-codes.h"

namespace STM32F103XB
{
	/**
	 * Драйвер UART.
	 */
	class UARTDriver
	{
	 private:
		/**
		 * Экземпляр драйвера UART3.
		 */
		static UARTDriver uart3;

		/**
		 * Регистр USART.
		 */
		USART_TypeDef& usart_register_;

		/**
		 * Драйвер RCC.
		 */
		RCCDriver* rcc_driver_ = nullptr;

		/**
		 * Драйвер канала DMA.
		 */
		DMADriver* dma_channel_driver_ = nullptr;

		/**
		 * Буфер передачи данных.
		 */
		Middleware::CircuitBuffer<DeviceConfig::UART3BufferSize> transmission_buffer_ = {};

		/**
		 * Флаг, предотвращающий повторную инициализацию UART.
		 */
		bool isInitialized = false;

		/**
		 * Инициализация драйвера UART.
		 * @param bus_speed Скорость шины APB, от которой тактируется UART
		 * @param baud_rate UART BaudRate
		 */
		Middleware::ReturnCode Init(uint32_t bus_speed, uint32_t baud_rate);

		explicit UARTDriver(USART_TypeDef& usart_register)
		: usart_register_(usart_register)
		{
		}
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера UART, если он не был создан ранее.
		 * @param usart_type Адрес регистра USART, который будем настраивать
		 * @param rcc_driver Драйвер RCC
		 * @param gpio_driver Драйвер GPIO
		 * @param dma_driver Драйвер DMA
		 * @param bus_speed Скорость шины APB, от которой тактируется UART
		 * @param baud_rate UART BaudRate
		 * @param out_uart_driver Драйвер UART
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(
			USART_TypeDef& usart_type,
			RCCDriver& rcc_driver,
			GPIODriver& gpio_driver,
			DMADriver& dma_driver,
			uint32_t bus_speed,
			uint32_t baud_rate,
			UARTDriver*& out_uart_driver);
		/**
		 * Обработать событие DMA Transaction Complete.
		 * @param dma_channel
		 */
		static void DMATransactionCompleteEventHandler(const DMADriver& dma_channel);

		uint32_t GetCurrentBaudRate();

		/**
		 * Отправить сообщение по UART.
		 * @param message_text Текст сообщения, ограниченный нуль-терминатором
		 */
		void SendMessage(const char message_text[]);

		/**
		 * Отправить сообщение по UART.
		 * @param message_text Текст сообщения, который может быть не ограничен нуль-терминатором.
		 * @param length Длина сообщения
		 */
		void SendMessage(const char message_text[], uint32_t length);
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_IO_UART_DRIVER_H_
