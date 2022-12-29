#ifndef SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_POWER_MODE_H_
#define SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_POWER_MODE_H_

namespace BMP280
{
	/**
	 * Режим работы.
	 */
	enum class PowerMode
	{
		/**
		 * Сон. Датчик практически не потребляет энергию, никакие измерения не проводятся.
		 */
		Sleep = 0x00,

		/**
		 * Принудительный. Датчик проводит единичное измерение согласно настроек конфигурации, и переходит в режим Сна.
		 */
		Forced = 0x02,

		/**
		 * Периодический. Датчик периодически просыпается, согласно настроек конфигурации, и делает измерения.
		 */
		Normal = 0x3
	};
}

#endif //SIMPLEMETEOSTATION_DRIVERS_BMP280_SOURCES_DATA_POWER_MODE_H_
