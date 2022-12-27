#include "rtc-driver.h"

namespace STM32F103XB
{
	RTCDriver RTCDriver::instance;

	Middleware::ReturnCode RTCDriver::SetConfigurationLockState(LockState configuration_lock_state)
	{
		system_timer_->Clear();
		switch(configuration_lock_state)
		{
			case LockState::Lock:
				RTC->CRL &= ~RTC_CRL_CNF_Msk;

				// Ждем окончание операций с RTC чтобы, выйти из конфигурационного режима.
				if (!system_timer_->WaitingForRegisterMaskSet(RTC->CRL,	RTC_CRL_RTOFF_Msk, RTC_CRL_RTOFF,
					DeviceConfig::RTCInitializationTimeoutMs))
				{
					return Middleware::ReturnCode::ERROR;
				}

				return Middleware::ReturnCode::OK;
			case LockState::Unlock:
				// Ждем окончание операций с RTC чтобы, войти в конфигурационный режим.
				if (!system_timer_->WaitingForRegisterMaskSet(RTC->CRL,	RTC_CRL_RTOFF_Msk, RTC_CRL_RTOFF,
					DeviceConfig::RTCInitializationTimeoutMs))
				{
					return Middleware::ReturnCode::ERROR;
				}

				RTC->CRL |= RTC_CRL_CNF;
				return Middleware::ReturnCode::OK;
			/**
			 * Всегда нужно проставлять default, что бы ни думал на этот счет Clang.
			 */
			#pragma clang diagnostic push
			#pragma ide diagnostic ignored "UnreachableCode"
			default:
				return Middleware::ReturnCode::ERROR;
			#pragma clang diagnostic pop
		}
	}

	Middleware::ReturnCode RTCDriver::ConfigureDivider()
	{
		auto status = SetConfigurationLockState(LockState::Unlock);
		if (Middleware::ReturnCode::OK != status)
		{
			logger_->Log(Application::LogLevel::Error, "%s", "RTC Configure Error: unlock configuration timeout.");
			return Middleware::ReturnCode::ERROR;
		}

		// Выставляем делитель под внешний кварц 32768Hz.
		RTC->PRLH = 0x0000;
		RTC->PRLL = 0x8000;

		status = SetConfigurationLockState(LockState::Lock);
		if (Middleware::ReturnCode::OK != status)
		{
			logger_->Log(Application::LogLevel::Error, "%s", "RTC Configure Error: lock configuration timeout.");
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode RTCDriver::SetAlarm(uint32_t seconds)
	{
		// Входим в Configuration Mode.
		auto configuration_lock_status = SetConfigurationLockState(LockState::Unlock);
		if (Middleware::ReturnCode::OK != configuration_lock_status)
		{
			logger_->Log(Application::LogLevel::Error, "%s", "RTC Configure Error: unlock configuration timeout.");
			return Middleware::ReturnCode::ERROR;
		}

		// Останавливаем часы, чтобы пока мы выставляем будильник он не ушел "в прошлое".
		// Проверка нужна затем, чтобы за то время, пока мы добирались сюда, время не убежало вперед нашего будильника.
		// Такое может случиться, если переданное количество секунд = 0.
		Stop();
		if (GetCurrentTime() > seconds)
		{
			Start();
			logger_->Log(Application::LogLevel::Error, "%s" , "RTC Set Alarm Error: Alarm cannot be set to a past date.");

			configuration_lock_status = SetConfigurationLockState(LockState::Lock);
			if (Middleware::ReturnCode::OK != configuration_lock_status)
			{
				logger_->Log(Application::LogLevel::Error, "%s", "RTC Configure Error: lock configuration timeout.");
			}
			return Middleware::ReturnCode::ERROR;
		}

		// Выставляем будильник.
		// Отнимаем единицу, т.к. при установке Alarm на текущее время мы будем разбужены через секунду, а если
		// установим Alarm на следующую секунду - то через две.
		RTC->ALRH = seconds >> 16;
		RTC->ALRL = seconds - 1;

		// Выходим из Configuration Mode.
		Start();
		system_timer_->Clear();
		configuration_lock_status = SetConfigurationLockState(LockState::Lock);
		if (Middleware::ReturnCode::OK != configuration_lock_status)
		{
			logger_->Log(Application::LogLevel::Error, "%s", "RTC Configure Error: lock configuration timeout.");
			return Middleware::ReturnCode::ERROR;
		}

		return Middleware::ReturnCode::OK;
	}

	Middleware::ReturnCode RTCDriver::CreateSingleInstance(
		SystemTimer& system_timer,
		UARTLogger& logger,
		RTCDriver*& out_rtc_timer_driver,
		bool cold_start)
	{
		instance.logger_ = &logger;

		// Контролируем уникальность экземпляра драйвера RTC.
		if (instance.isInitialized)
		{
			logger.Log(Application::LogLevel::Error, "%s", "RTC Initialization error: reinitialization.");
			return Middleware::ReturnCode::ERROR;
		}

		instance.system_timer_ = &system_timer;
		instance.system_timer_->Clear();

		auto status = instance.Init(cold_start);

		if (Middleware::ReturnCode::OK != status)
		{
			return Middleware::ReturnCode::ERROR;
		}

		instance.isInitialized = true;

		out_rtc_timer_driver = &instance;
		return Middleware::ReturnCode::OK;
	}



	Middleware::ReturnCode RTCDriver::Init(bool cold_start)
	{
		// Подаем тактирование на Power Interface:
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;

		// Снимаем защиту с записи битов Backup Domain Control Register.
		PWR->CR |= PWR_CR_DBP;

		// Инициализируем BDCR, если это первый запуск, а не выход из Standby-режима.
		// Или если мы хотим в любом случае провести инициализацию LSE, даже если она будет повторной.
		if (cold_start || !(PWR->CSR & PWR_CSR_SBF_Msk))
		{
			// Сбрасываем предыдущие значения BDCR.
			RCC->BDCR |= RCC_BDCR_BDRST;
			RCC->BDCR &= ~RCC_BDCR_BDRST_Msk;

			// Запускаем внешний кварц 32KHz.
			RCC->BDCR |= RCC_BDCR_LSEON;

			// Ждем включения LSE.
			if (!system_timer_->WaitingForRegisterMaskSet(
				RCC->BDCR,
				RCC_BDCR_LSERDY_Msk,
				RCC_BDCR_LSERDY,
				DeviceConfig::RTCInitializationTimeoutMs))
			{
				logger_->Log(Application::LogLevel::Error, "%s", "RTC Operation Timeout: waiting for LSE Ready.");
				return Middleware::ReturnCode::ERROR;
			}

			// Источник тактирования RTC.
			RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
		}

		Start();
		Middleware::ReturnCode status = ConfigureDivider();

		if (Middleware::ReturnCode::OK != status)
		{
			return status;
		}

		// Включаем прерывание по будильнику.
		RTC->CRH |= RTC_CRH_ALRIE;

		// Устанавливаем внешнее прерывание по будильнику.
		EXTI->IMR |= EXTI_IMR_MR17;
		EXTI->RTSR |= EXTI_RTSR_TR17;

		// Устанавливаем приоритет прерываний
		NVIC_SetPriority(RTC_Alarm_IRQn, DeviceConfig::RTCAlarmInterruptPriority);
		NVIC_EnableIRQ(RTC_Alarm_IRQn);

		return status;
	}

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	uint32_t RTCDriver::GetCurrentTime()
	{
		return (RTC->CNTH << 16) + RTC->CNTL;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void RTCDriver::Start()
	{
		// Включаем тактирование RTC.
		RCC->BDCR |= RCC_BDCR_RTCEN;
	}
	#pragma clang diagnostic pop

	/**
	 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
	 * экземпляром класса.
	 */
	#pragma clang diagnostic push
	#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	void RTCDriver::Stop()
	{
		// Выключаем тактирование RTC.
		RCC->BDCR &= ~RCC_BDCR_RTCEN_Msk;
	}
	#pragma clang diagnostic pop

	extern "C"
	{
		/**
 		* Обработка прерываний от EXTI.
 		*/
		[[maybe_unused]] void RTC_Alarm_IRQHandler(void)
		{
			// Нужен нам только для того, чтобы проснуться, поэтому просто сбрасываем флаг.
			EXTI->PR |= 1 << EXTI_PR_PR17_Pos;
		}
	}
}