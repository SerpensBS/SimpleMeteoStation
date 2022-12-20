#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_SOURCES_COLLECTIONS_CIRCUIT_BUFFER_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_SOURCES_COLLECTIONS_CIRCUIT_BUFFER_H_

#include "cmsis/stm/stm32f1xx.h"

namespace Middleware
{
	/**
	 * Кольцевой буфер.
	 */
	template <uint32_t buffer_size>
	class CircuitBuffer
	{
	 private:
		/**
		 * Хранилище данных буфера.
		 */
		char buffer[buffer_size] = {};

		/**
		 * Индекс первой непрочитанной единицы данных.
		 */
		uint32_t head_index_ = 0x00;

		/**
		 * Индекс в буфере, куда будет записана следующая единица данных.
		 */
		uint32_t tail_index_ = 0x00;

	 public:
		CircuitBuffer() = default;
		~CircuitBuffer() = default;

		/**
	 	 * Проверяет, пустой ли буфер.
	 	 * @return true -> буфер пуст
	 	 */
		[[nodiscard]] bool BufferIsEmpty() const;

		/**
	 	 * Получить единицу данных из буфера.
	 	 * @return Байт данных из буфера. Если буфер пуст, вернется нуль-терминатор.
	 	 */
		char* GetHead();

		/**
	 	 * Положить данные в буфер.
	 	 * @param byte Данные
	 	 */
		void PutTail(char byte);

		/**
		 * Положить набор данных в буфер.
		 * @param data Массив данных
		 * @param data_length Количество данных в массиве
		 */
		void PutTail(const char* data, uint32_t data_length);

		CircuitBuffer(const CircuitBuffer&) = delete;
		CircuitBuffer(const CircuitBuffer&&) = delete;
		CircuitBuffer& operator=(const CircuitBuffer&) = delete;
		CircuitBuffer& operator=(const CircuitBuffer&&) = delete;
	};


}
#include "circuit-buffer.inl"


#endif //SIMPLEMETEOSTATION_MIDDLEWARE_SOURCES_COLLECTIONS_CIRCUIT_BUFFER_H_
