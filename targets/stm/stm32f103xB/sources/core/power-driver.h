#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_

#include "application/interfaces/isleep.h"
#include "cmsis/stm/stm32f1xx.h"
#include "middleware/data/enums/return-codes.h"
#include "sources/memory/dma-driver.h"
#include "sources/timers/rtc-driver.h"
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
		 * Драйвер DMA.
		 */
		DMADriver* dma_driver_ = nullptr;

		/**
		 * Драйвер RTC.
		 */
		RTCDriver* rtc_driver_ = nullptr;

		/**
		 * Logger.
		 */
		UARTLogger* logger_ = nullptr;

		/**
		 * Флаг, предотвращающий повторную инициализацию драйвера управления питанием.
		 */
		bool isInitialized = false;

		PowerDriver() = default;
		~PowerDriver() override = default;

		/**
		 * Переводит контроллер в Sleep Mode. Выход из Sleep Mode по любому разрешенному прерыванию.
		 */
		void SleepMCU();

		/**
		 * Переводит контроллер в Stop Mode.
		 * @param stop_time_sec Время остановки в секундах
		 */
		void StopMCU(uint32_t stop_time_sec);
	 public:
		/**
		 * Создает и инициализирует единственный экземпляр драйвера управления питанием, если он не был создан ранее.
		 * @param dma_driver		Драйвер DMA
		 * @param rtc_driver		Драйвер RTC
		 * @param logger			Logger
		 * @param out_power_driver 	Инициализированный экземпляр драйвера управления питанием.
		 * 						 	Если драйвер уже был проинициализирован ранее - вернет nullptr.
		 * @return Статус операции
		 */
		static Middleware::ReturnCode CreateSingleInstance(
			DMADriver& dma_driver,
			RTCDriver& rtc_driver,
			UARTLogger& logger,
			PowerDriver*& out_power_driver);

		/**
		 * Останавливает контроллер и переводит его в режим пониженного энергопотребления.
		 * @param sleep_time_sec Время сна в секундах
		 */
		void Sleep(uint32_t sleep_time_sec) override;
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_SOURCES_CORE_POWER_DRIVER_H_
