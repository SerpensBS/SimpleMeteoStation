#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_

#include "application/interfaces/isleep.h"
#include "cmsis/stm/stm32f1xx.h"
#include "middleware/data/enums/return-codes.h"
#include "sources/timers/system-timer.h"

namespace STM32F103XB
{
	/**
	 * Драйвер управления питанием.
	 */
	class PowerDriver : public Application::ISleep
	{
	 private:
		/**
		 * Singleton экземпляр драйвера управления питанием.
		 */
		static PowerDriver instance;

		/**
		 * Системный таймер.
		 */
		SystemTimer* system_timer_ = nullptr;

		/**
		 * Флаг, предотвращающий повторную инициализацию драйвера управления питанием.
		 */
		bool isInitialized = false;

		PowerDriver() = default;
		~PowerDriver() override = default;

		/**
		 * Переводит контроллер в Sleep Mode.
		 * @param sleep_time_ms Время сна в миллисекундах.
		 */
		void SleepController(uint32_t sleep_time_ms);

		/**
		 * Переводит контроллер в Stop Mode.
		 * @param stop_time_s Время остановки в секундах
		 */
		void StopController(uint32_t stop_time_s);
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера управления питанием, если он не был создан ранее.
		 * @param system_timer 		Системный таймер
		 * @param out_power_driver 	Инициализированный экземпляр драйвера управления питанием.
		 * 						 	Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(SystemTimer& system_timer, PowerDriver*& out_power_driver);

		/**
		 * Переключает параметр SleepOnExit - по окончании прерывания контроллера продолжит работу, а не уйдет в сон.
		 */
		static void SleepOnExitModeOFF();

		/**
		 * Останавливает контроллер и переводит его в режим пониженного энергопотребления.
		 * @param sleep_time_ms Время сна в миллисекундах
		 */
		void Sleep(uint32_t sleep_time_ms) override;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_
