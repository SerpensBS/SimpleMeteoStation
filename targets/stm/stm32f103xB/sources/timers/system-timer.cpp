#include "config/device-config.h"
#include "system-timer.h"

namespace STM32F103XB
{
	SystemTimer SystemTimer::instance;

	Middleware::ReturnCode SystemTimer::CreateSingleInstance(RCCDriver& rcc_driver, SystemTimer*& out_system_timer)
	{
		// Контролируем уникальность экземпляра драйвера RCC.
		if (instance.isInitialized_)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.Init();

		// Определяем драйвер RCC и устанавливаем флаг контроля инициализации.
		instance.rcc_driver_ = &rcc_driver;
		instance.isInitialized_ = true;

		out_system_timer = &instance;
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
		SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos)	// Используем частоту шины AHB в качестве источника.
			| (1 << SysTick_CTRL_TICKINT_Pos);				// Генерируем прерывания по переполнению.

		// Настраиваем задержку, чтобы 1 тик был равен 1мс.
		SysTick->LOAD = rcc_driver_->GetAHBClock() / 1000;

		// Настраиваем приоритет.
		NVIC_SetPriority(SysTick_IRQn, DeviceConfig::SystemTimerInterruptPriority);
		NVIC_EnableIRQ(SysTick_IRQn);
	}
	#pragma clang diagnostic pop

	void SystemTimer::Restart()
	{
		SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk	// Отключаем таймер.
			| SysTick_CTRL_COUNTFLAG_Msk);			// Очищаем флаги переполнения.

		// Сбрасываем предыдущее значение таймера и запускаем.
		Clear();
		Start();
	}

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void SystemTimer::Start()
	{
		// Запускаем таймер.
		SysTick->CTRL |= 1 << SysTick_CTRL_ENABLE_Pos;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 * Также блокируем warning на квалификатор const, т.к. константная сигнатура метода может ввести в заблуждение,
	 * что ничего не изменяется, тогда как на самом деле мы изменяем с регистр таймера.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	#pragma ide diagnostic ignored "readability-make-member-function-const"
	uint32_t SystemTimer::Stop()
	{
		// Останавливаем таймер.
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

		return GetValue();
	}
	#pragma clang diagnostic pop

	uint32_t SystemTimer::GetValue() const
	{
		return counter_ms_;
	}

	void SystemTimer::SystemTimerInterruptHandler()
	{
		++instance.counter_ms_;
	}

	uint32_t SystemTimer::WaitingForRegisterMaskSet(
		volatile uint32_t& register_address,
		uint32_t bit_mask,
		uint32_t expected,
		uint32_t timeout_ms)
	{
		Start();

		// Ждем выставки битов по маске или истечение отведенного на операцию времени.
		while((register_address & bit_mask) != expected && timeout_ms > GetValue())
		{
		}

		Stop();
		return GetValue() > timeout_ms
			? 0
			: timeout_ms - GetValue();
	}

	void SystemTimer::Clear()
	{
		counter_ms_ = 0;
		SysTick->VAL = 0x00;
	}

	extern "C"
	{
		[[maybe_unused]] void SysTick_Handler()
		{
			SystemTimer::SystemTimerInterruptHandler();
		}
	}
}