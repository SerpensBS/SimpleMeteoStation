#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_RTC_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_RTC_DRIVER_H_

#include "middleware/data/enums/return-codes.h"
#include "sources/timers/system-timer.h"
#include "sources/utils/uart-logger.h"
#include "data/enums/lock-state.h"

namespace STM32F103XB
{
	/**
	 * Драйвер часов реального времени.
	 */
	class RTCDriver
	{
	 private:
		/**
		 * Singleton экземпляр часов реального времени.
		 */
		static RTCDriver instance;

		/**
		 * Системный таймер.
		 */
		SystemTimer* system_timer_ = nullptr;

		/**
		 * Logger.
		 */
		UARTLogger* logger_ = nullptr;

		/**
		 * Флаг, предотвращающий повторную инициализацию драйвера системного таймера.
		 */
		bool isInitialized = false;

		/**
		 * Настраиваем делитель под источник тактирования RTC.
		 * @return Статус операции.
		 */
		Middleware::ReturnCode ConfigureDivider();

		/**
		 * Заблокировать или разблокировать конфигурацию RTC.
		 * @param configuration_lock_state 	Статус блокировки конфигурации RTC
		 * @return  						Статус операции
		 */
		Middleware::ReturnCode SetConfigurationLockState(LockState configuration_lock_state);
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера часов реального времени, если он не был создан ранее.
		 * @param system_timer			Системный таймер
		 * @param logger				Logger
		 * @param out_rtc_timer_driver 	Инициализированный экземпляр драйвера часов реального времени.
		 * 						 		Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 * @param cold_start 			Нужно ли инициализировать RTC полностью с нуля?
		 * 								Часть конфигурации могла остаться с прошлого запуска.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(
			SystemTimer& system_timer,
			UARTLogger& logger,
			RTCDriver*& out_rtc_timer_driver,
			bool cold_start = false);

		/**
		 * Получить значение времени, которое отсчитал RTC с момента запуска.
		 * @return Количество секунд с момента запуска RTC.
		 */
		uint32_t GetCounterValue();

		/**
		 * Инициализация RTC.
		 * @param cold_start 	Нужно ли инициализировать RTC полностью с нуля?
		 * 						Часть конфигурации могла остаться с прошлого запуска.
		 * @return Статус операции
		 */
		Middleware::ReturnCode Init(bool cold_start);

		/**
	 	* Установить будильник.
		 * @param seconds Время в секундах с момента старта контроллера, в которое должен сработать будильник.
	 	* @return Статус операции
	 	*/
		Middleware::ReturnCode SetAlarm(uint32_t seconds);

		/**
		 * Запустить RTC.
		 */
		void Start();

		/**
		 * Остановить RTC.
		 */
		void Stop();
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_TIMERS_RTC_DRIVER_H_
