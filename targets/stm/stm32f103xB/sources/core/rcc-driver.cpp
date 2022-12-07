#include "rcc-driver.h"

namespace STM32F103XB
{
	RCCDriver* RCCDriver::instance_ = nullptr;

	/**
	 * Создает и инициализирует единственный экземпляр драйвера RCC, если он не был создан ранее.
	 * @param target_HCLK Целевая частота AHB
	 * @param out_rcc_driver Инициализированный экземпляр драйвера RCC. Если драйвер уже был проинициализирован ранее - вернет nullptr.
	 * @return Статус операции
	 */
	Middleware::ReturnCode RCCDriver::CreateSingleInstance(uint32_t target_HCLK, RCCDriver*& out_rcc_driver)
	{
		// Контролируем уникальность экземпляра драйвера RCC.
		if (nullptr == instance_)
		{
			static RCCDriver static_rcc_driver;
			*&instance_ = &static_rcc_driver;

			auto status = instance_->Init(target_HCLK);

			if (Middleware::ReturnCode::OK == status)
			{
				out_rcc_driver = instance_;
				return Middleware::ReturnCode::OK;
			}

			delete instance_;
			instance_ = nullptr;
		}

		return Middleware::ReturnCode::ERROR;
	}

	/**
	 * Инициализация RCC.
	 * @param target_HCLK Целевая частота AHB.
	 * @return Статус операции
	 */
	Middleware::ReturnCode RCCDriver::Init(uint32_t target_HCLK)
	{
		// Если по каким-то причинам HSI отключен, значит, что-то совсем пошло не так - выходим с ошибкой.
		if (!(RCC->CR & RCC_CR_HSIRDY))
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Устанавливаем делитель AHB.
		auto status = SetABHPrescaler(target_HCLK);
		if (Middleware::ReturnCode::OK != status)
		{
			return status;
		}

		return Middleware::ReturnCode::OK;
	}

/**
 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
 * экземпляром класса.
 */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	/**
	 * Устанавливает пред-делитель AHB.
	 * @param target_HCLK Целевая частота AHB
	 * @return Статус операции
	 */
	Middleware::ReturnCode RCCDriver::SetABHPrescaler(uint32_t target_HCLK)
	{
		uint32_t HPRE_value = GetAHBPrescalerValue(target_HCLK);

		// Если значение = 0xFF - значит, не смогли рассчитать делитель.
		if (0xFF == HPRE_value)
		{
			return Middleware::ReturnCode::ERROR;
		}

		RCC->CFGR |= HPRE_value << RCC_CFGR_HPRE_Pos;

		return Middleware::ReturnCode::OK;
	}
#pragma clang diagnostic pop

/**
 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
 * экземпляром класса.
 */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	/**
	 * Возвращает частоту APB1 (PCLK1).
	 * @return PCLK1
	 */
	[[maybe_unused]] uint32_t RCCDriver::GetAPB1Clock()
	{
		// Получаем частоту AHB.
		SystemCoreClockUpdate();
		// Рассчитываем APB1 по таблице.
		return SystemCoreClock>>APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];
	}
#pragma clang diagnostic pop

/**
 * Я бы не хотел, чтобы этот метод можно было вызвать статически, но он должен быть доступен коду, владеющему
 * экземпляром класса.
 */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	/**
	 * Возвращает частоту APB2 (PCLK2).
	 * @return PCLK2
	 */
	[[maybe_unused]] uint32_t RCCDriver::GetAPB2Clock()
	{
		// Получаем частоту AHB.
		SystemCoreClockUpdate();
		// Рассчитываем APB2 по таблице.
		return SystemCoreClock>>APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos];
	}
#pragma clang diagnostic pop

	/**
	 * Рассчитывает требуемое значение RCC_CFGR_HPRE в зависимости от значения SYSCLK.
	 * @param tgt_sysclk Целевая частота AHB.
	 * @return
	 */
	uint32_t RCCDriver::GetAHBPrescalerValue(uint32_t tgt_sysclk)
	{
		SystemCoreClockUpdate();
		uint32_t result = RCC_CFGR_HPRE_DIV2 >> RCC_CFGR_HPRE_Pos;

		// Если целевая частота равна частоте SYSCLK - значит, делитель не нужен.
		if (tgt_sysclk == SystemCoreClock)
		{
			return 0;
		}

		uint32_t div = 2;

		while(div <= 512)
		{
			if (SystemCoreClock / div == tgt_sysclk)
			{
				// После 16 вместо 32 идет сразу 64, при этом инкремент не меняется. См RM0008 стр. 103 Bits 7:4 HPRE.
				if (div > 16)
				{
					--result;
				}
				return result;
			}

			div = div << 1;
			++result;
		}

		return 0xFF;
	}
}