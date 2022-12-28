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
		static constexpr uint32_t TargetHCLK = 2000000UL;
		// Частота APB1 не может быть меньше чем 2MHz из-за ограничений I2C Slow Mode: RM0008 стр 758.
		// В проекте не используемся делитель частоты, так что APB1 == AHB.
		static_assert(2000000UL <= TargetHCLK);

		/**
		 * BaudRate, на который будет настроен UART3.
		 */
		static constexpr uint32_t UART3BaudRate = 115200;

		/**
		 * Размер буфера UART3.
		 */
		static constexpr uint32_t UART3BufferSize = 256;

		/**
		 * Размер буфера отправки каналов DMA.
		 */
		static constexpr uint32_t DMAChannelsTransactionBufferSize = 128;

		/**
		 * Предельное время инициализации RTC в миллисекундах.
		 */
		static constexpr uint32_t RTCInitializationTimeoutMs = 5000;

		/**
		 * Предельное время на одну транзакцию I2C в миллисекундах.
		 */
		static constexpr uint32_t I2CTransactionTimeoutMs = 5;

		/**
		 * Приоритет прерываний DMA1->Channel2. Обрабатывает отправку в UART.
		 */
		static constexpr uint32_t DMAChannel2InterruptPriority = 0;

		/**
		 * Приоритет прерываний системного таймера.
		 */
		static constexpr uint32_t SystemTimerInterruptPriority = 0;

		/**
		 * Приоритет прерывания по будильнику RTC.
		 */
		static constexpr uint32_t RTCAlarmInterruptPriority = 0;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
