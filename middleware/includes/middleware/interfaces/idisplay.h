#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IDISPLAY_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IDISPLAY_H_

#include <cstdint>
#include "middleware/data/enums/return-codes.h"

namespace Middleware
{
	/**
	 * Интерфейс драйвера дисплея.
	 */
	class IDisplay
	{
	 public:
		virtual ~IDisplay() = default;

		/**
		 * Очищает экран.
		 * @return Статус операции
		 */
		virtual ReturnCode ClearScreen() = 0;

		/**
		 * Выводит сообщение на дисплей.
		 * @param message Текст сообщения
		 * @param message_length Количество байт в сообщении
		 * @param row Строка на дисплее, на которую выводим сообщение
		 * @param column Столбец на дисплее, на котором выводим сообщение.
		 * @return Статус операции
		 */
		virtual ReturnCode PrintMessage(const char* message, uint32_t message_length, uint8_t row, uint8_t column) = 0;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IDISPLAY_H_
