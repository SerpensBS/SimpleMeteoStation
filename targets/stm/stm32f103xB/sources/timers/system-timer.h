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
		bool isInitialized = false;

		/**
		 * Действие при прерывании SysTick_IRQ.
		 */
		void (*interrupt_action_)() = nullptr;

		/**
		 * Инициализация системного таймера.
		 */
		inline void Init();

		/**
		 * Запустить таймер.
		 */
		inline void Start();

		/**
		 * Остановить таймер.
		 */
		inline void Stop();

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
		static void InterruptHandler();

		/**
		 * Начать обратный отсчет, по завершении которого запустить action.
		 * @param time_ms Время в миллисекундах, по прошествии которого будет запущен action
		 * @param callback_func Функция, которая будет запущена по окончанию отсчета таймера
		 * @return Статус операции
		 */
		Middleware::ReturnCode CountDown(uint32_t time_ms, void (*callback_func)());

		/**
		 * Передает, работает ли таймер в данный момент.
		 * @return Запущен ли таймер. True - таймер запущен. False - таймер не запущен
		 */
		bool IsRunning();
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_SYSTEM_TIMER_H_
