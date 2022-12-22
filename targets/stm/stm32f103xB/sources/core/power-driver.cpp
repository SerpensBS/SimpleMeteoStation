#include "power-driver.h"
#include "memory/dma-driver.h"

namespace STM32F103XB
{
	PowerDriver PowerDriver::instance;

	void PowerDriver::SleepController(uint32_t sleep_time_ms)
	{
		// Сбрасываем бит глубокого сна.
		SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

		// Уходим в сон. Настраиваем прерывания так, чтобы по выходу из них мы снова уходили в сон.
		SCB->SCR |= 1 << SCB_SCR_SLEEPONEXIT_Pos;

		// Устанавливаем будильник и уходим в сон.
		system_timer_->CountDown(sleep_time_ms, SleepOnExitModeOFF);
		__WFI();
	}

	void PowerDriver::StopController(uint32_t stop_time_s)
	{
		// Выставляем бит глубокого сна.
		SCB->SCR |= 1 << SCB_SCR_SLEEPDEEP_Pos;

		// Сбрасываем Power down deep sleep.
		PWR->CR &= ~PWR_CR_PDDS_Msk;

		// Если необходимо, устанавливаем режим low-power deep-sleep:
		PWR->CR |= 1 << PWR_CR_LPDS_Pos;

		// Включаем внешние прерывания.
		EXTI->IMR |= EXTI_IMR_MR17;
		EXTI->RTSR |= EXTI_RTSR_TR17;
		NVIC_EnableIRQ(RTC_Alarm_IRQn);

		// Уходим в режим остановки.
		SCB->SCR |= 1 << SCB_SCR_SLEEPONEXIT_Pos;
		__WFI();
	}

	void PowerDriver::Sleep(uint32_t sleep_time_ms)
	{
		uint32_t sleep_time_s = sleep_time_ms / 1000;
		sleep_time_ms = sleep_time_ms - sleep_time_s;

		if (sleep_time_s > 0)
		{
			StopController(sleep_time_s);
		}

		if (sleep_time_ms > 0)
		{
			SleepController(sleep_time_ms);
		}
	}

	Middleware::ReturnCode PowerDriver::CreateSingleInstance(SystemTimer& system_timer, PowerDriver*& out_power_driver)
	{
		// Контролируем уникальность экземпляра драйвера управления питанием.
		if (instance.isInitialized)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.system_timer_ = &system_timer;

		// Устанавливаем флаг контроля инициализации.
		instance.isInitialized = true;

		out_power_driver = &instance;
		return Middleware::ReturnCode::OK;
	}

	void PowerDriver::SleepOnExitModeOFF()
	{
		SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
	}
}