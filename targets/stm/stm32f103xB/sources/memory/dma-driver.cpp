#include "dma-driver.h"
#include "io/uart-driver.h"

namespace STM32F103XB
{
	DMADriver DMADriver::dma_channel2_driver(DMADriver(*DMA1_Channel2));

	Middleware::ReturnCode DMADriver::CreateSingleInstance(
		DMA_Channel_TypeDef& dma_channel_type,
		DMADriver*& out_dma_driver)
	{
		// Определяем, какой канал DMA нам нужен, и контролируем уникальность экземпляра драйвера.
		if (&dma_channel_type == DMA1_Channel2 && !dma_channel2_driver.isInitialized)
		{
			// Настраиваем DMA:
			dma_channel2_driver.Init();
			out_dma_driver = &dma_channel2_driver;

			return Middleware::ReturnCode::OK;
		}

		return Middleware::ReturnCode::ERROR;
	}

	bool DMADriver::IsRunning() const
	{
		return (dma_channel_register_.CCR & DMA_CCR_EN_Msk);
	}

	Middleware::ReturnCode DMADriver::Init()
	{
		// Включаем тактирование DMA.
		if (DMA1_Channel2 == &dma_channel_register_)
		{
			RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		}

		// Отключаем канал DMA.
		dma_channel_register_.CCR &= ~DMA_CCR_EN_Msk;

		// Настраиваем DMA:
		dma_channel_register_.CCR &= ~(
			DMA_CCR_MEM2MEM_Msk			// Отключаем MEM2MEM.
				| DMA_CCR_PL_Msk		// Низкий приоритет канала.
				| DMA_CCR_MSIZE_Msk		// Разрядность данных в SRAM.
				| DMA_CCR_PSIZE_Msk		// Разрядность данных в USART.
				| DMA_CCR_PINC_Msk		// Отключаем инкремент адреса периферии.
				| DMA_CCR_CIRC_Msk);	// Отключаем кольцевой режим.

		dma_channel_register_.CCR |=
				DMA_CCR_MINC			// Включаем инкремент адреса в SRAM.
				| DMA_CCR_TCIE			// Включаем прерывания по DMA Transfer Complete.
				| DMA_CCR_TEIE			// Включаем прерывания DMA.
				| DMA_CCR_DIR;			// Направление: SRAM -> USART.

		// Включаем прерывания.
		NVIC_SetPriority(DMA1_Channel2_IRQn, DeviceConfig::DMAChannel2InterruptPriority);
		NVIC_EnableIRQ(DMA1_Channel2_IRQn);

		dma_channel2_driver.isInitialized = true;
		return Middleware::ReturnCode::OK;
	}

	void DMADriver::SendDMA(volatile uint32_t destination_address, const char* data_message, uint32_t data_count)
	{
		if (!data_count)
		{
			// Вычисляем длину сообщения.
			while(data_message[data_count] != '\0')
			{
				++data_count;
			}

			if (!data_count)
			{
				return;
			}
		}

		for (
			uint32_t i = 0;
			'\0' != data_message[i]  && DeviceConfig::DMAChannelsTransactionBufferSize > i;
			++i)
		{
			dma_channel_transaction_buffer_[i] = data_message[i];
		}

		Send(destination_address, dma_channel_transaction_buffer_, data_count);
	}

	void DMADriver::Send(volatile uint32_t destination_address, const char* data_message, uint32_t data_count)
	{
		// Отключаем канал DMA:
		dma_channel_register_.CCR &= ~DMA_CCR_EN_Msk;

		// Передаем адреса в DMA:
		dma_channel_register_.CPAR = destination_address;
		dma_channel_register_.CMAR = reinterpret_cast<uint32_t>(data_message);
		dma_channel_register_.CNDTR = data_count;

		// Запускаем передачу DMA.
		dma_channel_register_.CCR |= DMA_CCR_EN_Msk;
	}

	/**
	 * Обрабатывает завершение передачи по каналу DMA.
	 * @param dma_channel Канал DMA
	 */
	inline void DMADriver::TransactionCompleteInterruptHandler(const DMA_Channel_TypeDef& dma_channel)
	{
		if (DMA1_Channel2 == &dma_channel)
		{
			DMADriver::dma_channel2_driver.dma_channel_register_.CCR &= ~DMA_CCR_EN_Msk;
			UARTDriver::DMATransactionCompleteEventHandler(dma_channel2_driver);
		}
	}

	extern "C"
	{
		/**
		 * Обработчик прерывания по 2 каналу DMA.
		 */
		[[maybe_unused]] void DMA1_Channel2_IRQHandler()
		{
			if (DMA1->ISR & DMA_IFCR_CTCIF2)
			{
				DMA1->IFCR |= DMA_IFCR_CTCIF2;
				DMADriver::TransactionCompleteInterruptHandler(*DMA1_Channel2);
			}
		}
	}
}



