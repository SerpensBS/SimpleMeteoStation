#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_

#include "cmsis/stm/stm32f1xx.h"
#include "middleware/data/configurations/logger-configuration.h"

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
		 * Конфигурация loggerr'а.
		 */
		static constexpr Middleware::LoggerConfiguration LoggerConfiguration
		{
			/**
			 * Минимальный уровень логирования.
			 */
			 Middleware::LogLevel::Trace,

			 /**
			  * Максимальный уровень логирования.
			  */
			  Middleware::LogLevel::Error
		};

		/**
		 * Размер буфера logger'а.
		 */
		static constexpr uint32_t StaticBufferLogSize = 100;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_DEVICE_CONFIG_H_
