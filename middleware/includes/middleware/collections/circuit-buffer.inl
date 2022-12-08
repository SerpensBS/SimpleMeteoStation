namespace Middleware
{
	template<uint32_t buffer_size>
	bool CircuitBuffer<buffer_size>::BufferIsEmpty() const
	{
		return head_index_ == tail_index_;
	}

	template<uint32_t buffer_size>
	void CircuitBuffer<buffer_size>::PutTail(char byte)
	{
		buffer[tail_index_] = byte;
		tail_index_++;

		if(buffer_size == tail_index_)
		{
			tail_index_ = 0;
		}
	}

	template<uint32_t buffer_size>
	void CircuitBuffer<buffer_size>::PutTail(const char* data, uint32_t data_length)
	{
		for (uint32_t i = 0; i < data_length; ++i)
		{
			PutTail(data[i]);
		}
	}

	template<uint32_t buffer_size>
	char* CircuitBuffer<buffer_size>::GetHead()
	{
		if (!BufferIsEmpty())
		{
			char& byte = buffer[head_index_];
			++head_index_;

			if(buffer_size == head_index_)
			{
				head_index_ = 0;
			}

			return &byte;
		}

		return nullptr;
	}
}
