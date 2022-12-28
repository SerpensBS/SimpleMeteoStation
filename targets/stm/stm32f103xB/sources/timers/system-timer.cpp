#include "config/device-config.h"
#include "system-timer.h"

namespace STM32F103XB
{
	uint32_t SystemTimer::system_timer_counter_ms = 0;
	uint32_t SystemTimer::started_timers_in_current_time = 0;

	void SystemTimer::Init(RCCDriver& rcc_driver)
	{
		SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos)	// Используем частоту шины AHB в качестве источника.
			| (1 << SysTick_CTRL_TICKINT_Pos);				// Генерируем прерывания по переполнению.

		// Настраиваем задержку, чтобы 1 тик был равен 1мс.
		SysTick->LOAD = rcc_driver.GetAHBClock() / 1000;

		SysTick->VAL = 0x00;

		// Настраиваем приоритет.
		NVIC_SetPriority(SysTick_IRQn, DeviceConfig::SystemTimerInterruptPriority);
		NVIC_EnableIRQ(SysTick_IRQn);
	}

	void SystemTimer::Restart()
	{
		Clear();
		Start();
	}

	void SystemTimer::Start()
	{
		// Очищаем флаги переполнения.
		SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;

		last_check_counter_time = system_timer_counter_ms;
		isRunning = true;
		++started_timers_in_current_time;

		// Запускаем таймер.
		SysTick->CTRL |= 1 << SysTick_CTRL_ENABLE_Pos;

	}

	uint32_t SystemTimer::Stop()
	{
		// Получаем результат и обновляем значение счетчика.
		uint32_t result = GetValue();

		// Останавливаем таймер.
		isRunning = false;
		--started_timers_in_current_time;

		// Если запущенных таймеров больше нет - останавливаем и обнуляем общий счетчик.
		if (!started_timers_in_current_time)
		{
			SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
			system_timer_counter_ms = 0;
		}

		return result;
	}

	uint32_t SystemTimer::GetValue()
	{
		if (isRunning)
		{
			counter_ms_ += system_timer_counter_ms - last_check_counter_time;
			last_check_counter_time = system_timer_counter_ms;
		}

		return counter_ms_;
	}

	void SystemTimer::SystemTimerInterruptHandler()
	{
		++system_timer_counter_ms;
	}

	uint32_t SystemTimer::WaitingForRegisterMaskSet(
		volatile uint32_t& register_address,
		uint32_t bit_mask,
		uint32_t expected,
		uint32_t timeout_ms,
		bool is_timer_restart)
	{
		// Если необходимо делать Restart - делаем.
		is_timer_restart
			? Restart()
			: Start();

		// Ждем выставки битов по маске или истечение отведенного на операцию времени.
		while((register_address & bit_mask) != expected && timeout_ms > GetValue())
		{
		}

		return Stop() > timeout_ms
			? 0
			: timeout_ms - GetValue();
	}

	void SystemTimer::Clear()
	{
		last_check_counter_time = system_timer_counter_ms;
		counter_ms_ = 0;
	}

	extern "C"
	{
		[[maybe_unused]] void SysTick_Handler()
		{
			SystemTimer::SystemTimerInterruptHandler();
		}
	}
}