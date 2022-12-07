#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_DISPLAY_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_DISPLAY_MOCK_H_

#include "middleware/interfaces/idisplay.h"

namespace ApplicationTests
{
	/**
	 * Mock для дисплея.
	 * @tparam width_display Ширина дисплея
	 * @tparam height_display Высота дисплея
	 */
	template<uint32_t width_display, uint32_t height_display>
	class DisplayMock : public Middleware::IDisplay
	{
	 private:
		/**
		 * Эмулирует матрицу дисплея.
		 */
		char display_screen_[height_display][width_display] = {};

		/**
		 * Вывести в терминал содержимое дисплея.
		 */
		void PrintScreen();
	 public:
		DisplayMock() = default;

		~DisplayMock() override = default;

		/**
		 * Очищает экран.
		 * @return Статус операции
		 */
		Middleware::ReturnCode ClearScreen() override;

		/**
		 * Выводит сообщение на дисплей.
		 * @param message Текст сообщения
		 * @param message_length Количество байт в сообщении
		 * @param row Строка на дисплее, на которую выводим сообщение
		 * @param column Столбец на дисплее, на котором выводим сообщение.
		 * @return Статус операции
		 */
		Middleware::ReturnCode PrintMessage(const char* message, uint32_t message_length, uint8_t row, uint8_t column) override;
	};
}

#include "display-mock.inl"

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_DISPLAY_MOCK_H_
