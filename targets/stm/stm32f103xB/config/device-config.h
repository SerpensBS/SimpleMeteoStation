#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_

#include "cmsis/stm/stm32f1xx.h"

namespace STM32F103XB
{
	/**
	 * Конфигурация устройства.
	 */
	struct DeviceConfig
	{
		/**
		 * Тактовая частота, на которую будет настроена AHB.
		 */
		static constexpr uint32_t TargetHCLK = 125000UL;

		/**
		 * BaudRate, на который будет настроен UART3.
		 */
		static constexpr uint32_t UART3BaudRate = 2400;

		/**
		 * Размер буфера UART3.
		 */
		static constexpr uint32_t UART3BufferSize = 256;

		/**
		 * Размер буфера отправки каналов DMA.
		 */
		static constexpr uint32_t DMAChannelsTransactionBufferSize = 128;

		/**
		 * Приоритет прерываний DMA1->Channel2. Обрабатывает отправку в UART.
		 */
		static constexpr uint32_t DMAChannel2InterruptPriority = 0;

		/**
		 * Приоритет прерываний системного таймера.
		 */
		static constexpr uint32_t SystemTimerInterruptPriority = 0;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
