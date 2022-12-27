#include "power-driver.h"

namespace STM32F103XB
{
	PowerDriver PowerDriver::instance;

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void PowerDriver::SleepMCU()
	{
		// Сбрасываем бит глубокого сна.
		SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

		// Уходим в сон. Настраиваем прерывания так, чтобы прерывания переводили нас обратно в Run Mode.
		SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

		// Уходим в сон.
		__WFI();
	}
	#pragma clang diagnostic pop

	void PowerDriver::StopMCU(uint32_t stop_time_sec)
	{
		// Ставим будильник и уходим в режим остановки.
		auto status = rtc_driver_->SetAlarm(rtc_driver_->GetCurrentTime() + stop_time_sec);

		if (Middleware::ReturnCode::OK != status)
		{
			logger_->Log(Application::LogLevel::Error, "Set Alarm Error. No entry to stop mode. Returned code: %d", status);
			return;
		}

		// Выставляем бит глубокого сна.
		SCB->SCR |= 1 << SCB_SCR_SLEEPDEEP_Pos;

		// Сбрасываем Power down deep sleep.
		PWR->CR &= ~PWR_CR_PDDS_Msk;

		// Устанавливаем режим low-power deep-sleep:
		PWR->CR |= 1 << PWR_CR_LPDS_Pos;

		// Настраиваем прерывания так, чтобы прерывания переводили нас обратно в Run Mode.
		SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

		__WFI();
	}

	void PowerDriver::Sleep(uint32_t sleep_time_sec)
	{
		uint32_t current_time = rtc_driver_->GetCurrentTime();

		// Если DMA в данный момент пересылает что-либо, переходим в сон до тех пор, пока обработка не будет закончена.
		while(dma_driver_->IsRunning())
		{
			SleepMCU();
		}

		// Если на предыдущем шаге мы не проспали время когда нам надо просыпаться - засыпаем.
		if (current_time < rtc_driver_->GetCurrentTime() + sleep_time_sec)
		{
			StopMCU(sleep_time_sec);
		}
	}

	Middleware::ReturnCode PowerDriver::CreateSingleInstance(
		DMADriver& dma_driver,
		RTCDriver& rtc_driver,
		UARTLogger& logger,
		PowerDriver*& out_power_driver)
	{
		// Контролируем уникальность экземпляра драйвера управления питанием.
		if (instance.isInitialized)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.dma_driver_ = &dma_driver;
		instance.rtc_driver_ = &rtc_driver;
		instance.logger_ = &logger;

		// Устанавливаем флаг контроля инициализации.
		instance.isInitialized = true;

		out_power_driver = &instance;
		return Middleware::ReturnCode::OK;
	}
}