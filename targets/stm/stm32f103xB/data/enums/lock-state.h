#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_LOCK_STATE_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_LOCK_STATE_H_

namespace STM32F103XB
{
	/**
	 * Статус блокировки объекта.
	 */
	enum class LockState
	{
		Lock,
		Unlock
	};
}

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_DATA_ENUMS_LOCK_STATE_H_