#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_RCC_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_RCC_DRIVER_H_

#include "cmsis/stm/stm32f1xx.h"
#include "middleware/enums/return-codes.h"

namespace STM32F103XB
{
	/**
	 * Драйвер для управления тактирование контроллера.
	 */
	class RCCDriver
	{
	 private:
		/**
		 * Singleton экземпляр RCC драйвера.
		 */
		static RCCDriver* instance;

		static uint32_t GetAHBPrescalerValue(uint32_t tgt_sysclk);

		Middleware::ReturnCode SetABHPrescaler(uint32_t target_HCLK);

		RCCDriver() = default;
		~RCCDriver() = default;
	 public:
		static Middleware::ReturnCode CreateSingleInstance(uint32_t target_HCLK, RCCDriver*& out_rcc_driver);

		Middleware::ReturnCode Init(uint32_t target_HCLK);
		[[maybe_unused]] uint32_t GetAPB1Clock();
		[[maybe_unused]] uint32_t GetAPB2Clock();

		RCCDriver(const RCCDriver&) = delete;
		RCCDriver(const RCCDriver&&) = delete;
		RCCDriver& operator=(const RCCDriver&) = delete;
		RCCDriver& operator=(const RCCDriver&&) = delete;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_RCC_DRIVER_H_