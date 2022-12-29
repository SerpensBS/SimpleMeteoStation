#ifndef SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_ABSTRACT_INTERFACES_ICOMMUNICATION_BUS_H_
#define SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_ABSTRACT_INTERFACES_ICOMMUNICATION_BUS_H_

#include <cstdint>
#include "middleware/data/enums/return-codes.h"

namespace BMP280
{
	/**
	 * Интерфейс шины обмена данными.
	 */
	class ICommunicationBus
	{
	 public:
		virtual Middleware::ReturnCode SendData(
			uint8_t slave_device_address,
			uint8_t data_register_address,
			const uint8_t* message,
			uint32_t message_size) = 0;

		virtual Middleware::ReturnCode ReadData(
			uint8_t slave_address,
			uint8_t slave_address_register,
			uint8_t* read_buffer,
			uint32_t receive_bytes_count) = 0;

		virtual ~ICommunicationBus() = default;
	};
}

#endif //SIMPLEMETEOSTATION_DRIVERS_BMP280_INCLUDES_ABSTRACT_INTERFACES_ICOMMUNICATION_BUS_H_
