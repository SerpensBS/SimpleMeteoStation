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
		 * Общий счетчик для всех таймеров.
		 */
		static uint32_t system_timer_counter_ms;

		/**
		 * Количество запущенных таймеров в текущий момент.
		 */
		static uint32_t started_timers_in_current_time;

		/**
		 * Счетчик миллисекунд, отсчитанных таймером.
		 */
		uint32_t counter_ms_ = 0;

		/**
		 * Время последней проверки общего счетчика.
		 */
		uint32_t last_check_counter_time = 0;

		/**
		 * Запущен ли таймер в данный момент.
		 */
		bool isRunning = false;
	 public:
		/**
		 * Инициализация системного таймера.
		 */
		static void Init(RCCDriver& rcc_driver);

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
		uint32_t GetValue();

		/**
		 * Обнулить и запустить таймер снова.
		 */
		void Restart();

		/**
		 * Запустить таймер.
		 */
		void Start();

		/**
		 * Остановить таймер.
		 */
		uint32_t Stop();

		/**
		 * Ожидаем установку значений в регистр по маске за отведенное время.
		 * @param register_address Адрес регистра
		 * @param bit_mask Битовая маска
		 * @param expected Ожидаемое значение
		 * @param timeout_ms Таймаут в миллисекундах
		 * @param need_timer_restart Таймер будет обнулен перед началом измерений
		 * @return Количество оставшегося времени
		 */
		uint32_t WaitingForRegisterMaskSet(
			volatile uint32_t& register_address,
			uint32_t bit_mask,
			uint32_t expected,
			uint32_t timeout_ms,
			bool is_timer_restart = false);

		SystemTimer() = default;
		~SystemTimer() = default;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_SYSTEM_TIMER_H_
