#include "config/device-config.h"
#include "system-timer.h"

namespace STM32F103XB
{
	SystemTimer SystemTimer::instance;

	Middleware::ReturnCode SystemTimer::CreateSingleInstance(RCCDriver& rcc_driver, SystemTimer*& out_system_timer)
	{
		// Контролируем уникальность экземпляра драйвера RCC.
		if (instance.isInitialized)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.Init();

		// Определяем драйвер RCC и устанавливаем флаг контроля инициализации.
		instance.rcc_driver_ = &rcc_driver;

		out_system_timer = &instance;
		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode SystemTimer::CountDown(uint32_t time_ms, void (*callback_func)())
	{
		if (IsRunning())
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Генерируем прерывания по переполнению.
		SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos);

		// Настраиваем задержку.
		SystemCoreClockUpdate();
		SysTick->LOAD = rcc_driver_->GetAHBClock() / 1000 * time_ms;

		// Настраиваем событие, вызываемое по истечению таймера.
		interrupt_action_ = callback_func;

		// Сбрасываем предыдущее значение таймера.
		SysTick->VAL = 0x00;

		// Запускаем таймер.
		Start();

		return Middleware::ReturnCode::OK;
	}

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void SystemTimer::Init()
	{
		// Используем частоту шины AHB в качестве источника.
		SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos);

		// Настраиваем приоритет.
		NVIC_SetPriority(SysTick_IRQn, DeviceConfig::SystemTimerInterruptPriority);
		NVIC_EnableIRQ(SysTick_IRQn);

		// Устанавливаем флаг контроля инициализации.
		instance.isInitialized = true;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	bool SystemTimer::IsRunning()
	{
		return SysTick->CTRL & SysTick_CTRL_ENABLE_Msk;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void SystemTimer::Start()
	{
		SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk	// Отключаем таймер.
			| SysTick_CTRL_COUNTFLAG_Msk);			// Очищаем флаги переполнения.

		// Сбрасываем предыдущее значение таймера.
		SysTick->VAL = 0x00;
		// Запускаем таймер.
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void SystemTimer::Stop()
	{
		// Останавливаем таймер.
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	}
	#pragma clang diagnostic pop

	inline void SystemTimer::InterruptHandler()
	{
		if (instance.interrupt_action_)
		{
			instance.interrupt_action_();
			instance.interrupt_action_ = nullptr;
		}

		instance.Stop();
	}

	extern "C"
	{
		[[maybe_unused]] void SysTick_Handler()
		{
			SystemTimer::InterruptHandler();
		}
	}
}