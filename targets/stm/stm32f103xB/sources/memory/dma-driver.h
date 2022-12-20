#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_MEMORY_DMA_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_MEMORY_DMA_DRIVER_H_

#include "cmsis/stm/stm32f1xx.h"
#include "config/device-config.h"
#include "middleware/collections/circuit-buffer.h"
#include "middleware/data/enums/return-codes.h"

namespace STM32F103XB
{
	/**
	 * Драйвер каналов DMA.
	 */
	class DMADriver
	{
	 private:
		/**
		 * Singleton второго канала DMA.
		 */
		static DMADriver dma_channel2_driver;

		/**
		 * Регистр канала DMA.
		 */
		DMA_Channel_TypeDef& dma_channel_register_;

		/**
		 * Буфер канала, в который помещаются данные перед отправкой.
		 */
		char dma_channel_transaction_buffer_[DeviceConfig::DMAChannelsTransactionBufferSize] = {};

		/**
		 * Флаг, предотвращающий повторную инициализацию канала DMA.
		 */
		bool isInitialized = false;

		/**
		 * Инициализировать отправку массива данных в DMA.
		 * @param destination_address Адрес назначения
		 * @param data_message Сообщение, которое необходимо отправить
		 * @param data_count Количество байт, которое будем отправлять
		 */
		void Send(volatile uint32_t destination_address, const char* data_message, uint32_t data_count);

		/**
		 * Конструктор драйвера канала DMA
		 * @param dma_channel_register Регистр канала DMA
		 */
		explicit DMADriver(DMA_Channel_TypeDef& dma_channel_register)
			: dma_channel_register_(dma_channel_register)
		{
		}
	 public:
		/**
		 * Инициализирует и возвращает единственный экземпляр класса драйвера канала DMA.
		 * Если экземпляр уже был инициализирован ранее - вернет nullptr.
		 * @param dma_channel_type Тип канала DMA
		 * @param out_dma_driver Драйвер канала DMA
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(DMA_Channel_TypeDef& dma_channel_type, DMADriver*& out_dma_driver);

		/**
		 * Обработка прерывания по событию Transaction Complete.
		 * @param dma_channel
		 */
		inline static void TransactionCompleteInterruptHandler(const DMA_Channel_TypeDef& dma_channel);

		/**
		 * Стартовая инициализация канала DMA.
		 * @return
		 */
		Middleware::ReturnCode Init();

		/**
		 * Занят ли в данный момент канал DMA.
		 * @return true - канал DMA занят. false - канал DMA свободен
		 */
		[[nodiscard]] bool DMAIsRunning() const;

		/**
		 * Инициализирует отправку массива данных в DMA из кольцевого буфера.
		 * @tparam size Размер кольцевого буфера
		 * @param destination_address Адрес назначения
		 * @param message_buffer Кольцевой буфер
		 */
		template<uint32_t size>
		void SendDMA(volatile uint32_t destination_address, Middleware::CircuitBuffer<size>& message_buffer);

		/**
		 * Инициализировать отправку массива данных в DMA.
		 * @param destination_address Адрес назначения
		 * @param data_message Сообщение, которое необходимо отправить
		 * @param data_count Количество байт, которое будем отправлять
		 */
		void SendDMA(volatile uint32_t destination_address, const char* data_message, uint32_t data_count = 0);

		DMADriver() = delete;
		DMADriver(const DMADriver&) = delete;
		DMADriver(const DMADriver&&) = delete;
		DMADriver& operator=(const DMADriver&) = delete;
		DMADriver& operator=(const DMADriver&&) = delete;
	};
}

#include "dma-driver.inl"
#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_MEMORY_DMA_DRIVER_H_
