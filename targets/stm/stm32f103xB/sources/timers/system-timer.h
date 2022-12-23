#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_SYSTEM_TIMER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_SYSTEM_TIMER_H_

#include "middleware/data/enums/return-codes.h"
#include "sources/core/rcc-driver.h"

namespace STM32F103XB
{
	class SystemTimer
	{
	 private:
		/**
		 * Singleton экземпляр драйвера системного таймера.
		 */
		static SystemTimer instance;

		/**
		 * Драйвер RCC.
		 */
		RCCDriver* rcc_driver_ = nullptr;

		/**
		 * Флаг, предотвращающий повторную инициализацию драйвера системного таймера.
		 */
		bool isInitialized_ = false;

		/**
		 * Счетчик миллисекунд, отсчитанных таймером.
		 */
		uint32_t counter_ms_ = 0;

		/**
		 * Инициализация системного таймера.
		 */
		inline void Init();

		SystemTimer() = default;
		~SystemTimer() = default;
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера системного таймера, если он не был создан ранее.
		 * @param out_system_timer Инициализированный экземпляр драйвера системного таймера.
		 * 						 Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(RCCDriver& rcc_driver, SystemTimer*& out_system_timer);

		/**
		 * Обработчик прерывания системного таймера.
		 */
		static void SystemTimerInterruptHandler();

		/**
		 * Очищает показания счетчика.
		 */
		void Clear();

		/**
		 * Получить количество миллисекунд, прошедших с момента запуска таймера.
		 * @return Количество миллисекунд, прошедших с момента запуска таймера.
		 */
		[[nodiscard]] inline uint32_t GetValue() const;

		/**
		 * Обнулить счетчик и перезапустить таймер.
		 */
		inline void Restart();

		/**
		 * Запустить таймер.
		 */
		inline void Start();

		/**
		 * Остановить таймер.
		 */
		inline uint32_t Stop();

		/**
		 * Ожидаем установку значений в регистр по маске за отведенное время.
		 * @param register_address Адрес регистра
		 * @param bit_mask Битовая маска
		 * @param expected Ожидаемое значение
		 * @param timeout_ms Таймаут в миллисекундах
		 * @return Количество оставшегося времени
		 */
		uint32_t WaitingForRegisterMaskSet(
			volatile uint32_t& register_address,
			uint32_t bit_mask,
			uint32_t expected,
			uint32_t timeout_ms);
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_SYSTEM_TIMER_H_
