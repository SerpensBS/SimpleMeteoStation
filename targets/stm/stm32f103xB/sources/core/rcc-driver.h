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

		/**
	 	 * Рассчитывает требуемое значение RCC_CFGR_HPRE в зависимости от значения SYSCLK.
	 	 * @param tgt_sysclk Целевая частота AHB.
	 	 * @return требуемое значение RCC_CFGR_HPRE
	 	 */
		static uint32_t GetAHBPrescalerValue(uint32_t tgt_sysclk);

		/**
	 	 * Устанавливает пред-делитель AHB.
	 	 * @param target_HCLK Целевая частота AHB
	 	 * @return Статус операции
	 	 */
		Middleware::ReturnCode SetABHPrescaler(uint32_t target_HCLK);

		RCCDriver() = default;
		~RCCDriver() = default;
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера RCC, если он не был создан ранее.
		 * @param target_HCLK Целевая частота AHB
		 * @param out_rcc_driver Инициализированный экземпляр драйвера RCC.
		 * 						 Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(uint32_t target_HCLK, RCCDriver*& out_rcc_driver);

		/**
	 	 * Инициализация RCC.
	 	 * @param target_HCLK Целевая частота AHB.
	 	 * @return Статус операции
	 	 */
		Middleware::ReturnCode Init(uint32_t target_HCLK);

		/**
	 	 * Возвращает частоту APB1 (PCLK1).
	 	 * @return PCLK1
	 	 */
		[[maybe_unused]] uint32_t GetAPB1Clock();

		/**
	 	 * Возвращает частоту APB2 (PCLK2).
	 	 * @return PCLK2
	 	 */
		[[maybe_unused]] uint32_t GetAPB2Clock();

		RCCDriver(const RCCDriver&) = delete;
		RCCDriver(const RCCDriver&&) = delete;
		RCCDriver& operator=(const RCCDriver&) = delete;
		RCCDriver& operator=(const RCCDriver&&) = delete;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_RCC_DRIVER_H_