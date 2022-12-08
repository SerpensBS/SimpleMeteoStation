namespace STM32F103XB
{
	template<uint32_t size>
	void DMADriver::SendDMAFromCircuitBuffer(volatile uint32_t destination_address,
		Middleware::CircuitBuffer<size>& message_buffer)
	{
		uint32_t message_length = 0;

		while(!message_buffer.BufferIsEmpty() && DeviceConfig::DMAChannelsTransactionBufferSize > message_length)
		{
			dma_channel_transaction_buffer_[message_length] = *message_buffer.GetHead();
			++message_length;
		}

		SendDMA(destination_address, dma_channel_transaction_buffer_, message_length);
	}
}