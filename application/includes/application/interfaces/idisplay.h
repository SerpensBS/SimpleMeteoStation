#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_IDISPLAY_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_IDISPLAY_H_

#include <cstdint>
#include "middleware/data/enums/return-codes.h"

namespace Application
{
	/**
	 * Интерфейс драйвера дисплея.
	 */
	class IDisplay
	{
	 public:
		/**
		 * Очищает экран.
		 * @return Статус операции
		 */
		virtual Middleware::ReturnCode ClearScreen() = 0;

		/**
		 * Выводит сообщение на дисплей.
		 * @param message Текст сообщения
		 * @param message_length Количество байт в сообщении
		 * @param row Строка на дисплее, на которую выводим сообщение
		 * @param column Столбец на дисплее, на котором выводим сообщение.
		 * @return Статус операции
		 */
		virtual Middleware::ReturnCode PrintMessage(
			const char* message,
			uint32_t message_length,
			uint8_t row,
			uint8_t column) = 0;

		virtual ~IDisplay() = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_IDISPLAY_H_
