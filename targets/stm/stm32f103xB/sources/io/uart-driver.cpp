#include "gpio-driver.h"
#include "uart-driver.h"

namespace STM32F103XB
{
	UARTDriver UARTDriver::uart3(UARTDriver(*USART3));

	Middleware::ReturnCode UARTDriver::CreateSingleInstance(
		USART_TypeDef& usart_type,
		RCCDriver& rcc_driver,
		GPIODriver& gpio_driver,
		DMADriver& dma_driver,
		uint32_t bus_speed,
		uint32_t baud_rate,
		UARTDriver*& out_uart_driver)
	{
		// Определяем, какой UART нам нужен, и контролируем уникальность экземпляра драйвера.
		if (&usart_type == USART3 && !uart3.isInitialized)
		{
			// Настраиваем GPIO:
			// Включаем тактирование порта B и его альтернативные функции.
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

			// Настраиваем передачу:
			GPIOPinConfiguration uart_tx_config = {*GPIOB, 10, AlternateFunctionPushPull, OutputMaxSpeed2MHz};
			auto status = gpio_driver.ConfigurePin(uart_tx_config);

			// Если не удалось сконфигурировать GPIO - выходим.
			if (Middleware::ReturnCode::OK != status)
			{
				return Middleware::ReturnCode::ERROR;
			}

			// Включаем тактирование UART3.
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

			uart3.dma_channel_driver_ = &dma_driver;
			uart3.rcc_driver_ = &rcc_driver;
			status = uart3.Init(bus_speed, baud_rate);

			if (Middleware::ReturnCode::OK == status)
			{
				uart3.isInitialized = true;
				out_uart_driver = &uart3;

				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	/**
 	* Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
 	* экземпляром класса.
 	*/
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	Middleware::ReturnCode UARTDriver::Init(
		uint32_t bus_speed,
		uint32_t baud_rate)
	{
		if (!dma_channel_driver_)
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Настраиваем Baud Rate.
		usart_register_.BRR = bus_speed / baud_rate;

		usart_register_.CR1 |= USART_CR1_UE	// Включаем тактирование USART.
			| USART_CR1_TE;						// Включаем передачу.

		// Сбрасываем флаг завершения передачи (по-умолчанию выставлен).
		usart_register_.SR &= ~USART_SR_TC_Msk;

		// Включаем первый канал DMA.
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		// Разрешаем передачу через DMA для USART3.
		usart_register_.CR3 |= USART_CR3_DMAT;

		return Middleware::ReturnCode::OK;
	}

	void UARTDriver::SendMessage(const char* message_text)
	{
		uint32_t message_length = 0;

		// Вычисляем длину сообщения.
		while(message_text[message_length] != '\0')
		{
			++message_length;
		}

		if (!message_length)
		{
			return;
		}

		SendMessage(message_text, message_length);
	}

	void UARTDriver::SendMessage(const char* message_text, uint32_t message_length)
	{
		// Если DMA в данный момент уже занят - кладем данные в буфер.
		if (dma_channel_driver_->DMAIsRunning())
		{
			transmission_buffer_.PutTail(message_text, message_length);
		}
		else
		{
			dma_channel_driver_->SendDMA(reinterpret_cast<uint32_t>(&usart_register_.DR),
				message_text,
				message_length);
		}
	}

	void UARTDriver::DMATransactionCompleteEventHandler(const DMADriver& dma_channel)
	{
		if (&dma_channel == uart3.dma_channel_driver_)
		{
			if(!uart3.transmission_buffer_.BufferIsEmpty())
			{
				uart3.dma_channel_driver_->SendDMA(
					reinterpret_cast<uint32_t>(&uart3.usart_register_.DR),
					uart3.transmission_buffer_);
			}
		}
	}

	uint32_t UARTDriver::GetCurrentBaudRate()
	{
		return rcc_driver_->GetAPB2Clock() / usart_register_.BRR;
	}

	#pragma clang diagnostic pop
}