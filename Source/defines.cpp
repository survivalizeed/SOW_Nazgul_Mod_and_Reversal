#include "pch.h"

namespace intern::OFFSETS {
	uintptr_t GLOW_FUNC_ADDRESS = 0x1405902AC;
	uintptr_t TIME_FUNC_ADDRESS = 0x1407A35D8;
	uintptr_t ENTITY_FUNC_ADDRESS = 0x14024FEA0;
	uintptr_t CAMERA_ADDRESS = 0x14273B390;
	uintptr_t PLAYER_BASE_ADDRESS = 0x1426FFB70;
	std::vector<unsigned int> PLAYER_OFFSETS = { 0x28, 0x468, 0x20, 0x8, 0x38, 0x90, 0xAC };
	uintptr_t TIME_FUNC_PARAM2_BASE_ADDRESS = 0x142A61130;
	std::vector<unsigned int> TIME_FUNC_PARAM2_OFFSETS = { 0xC8, 0x10, 0x50, 0x40, 0xE8, 0x38, 0xB58 };
}

namespace intern::GLOW {
	GLOW_FUNC function = NULL;
	GLOW_FUNC dFunction = (GLOW_FUNC)OFFSETS::GLOW_FUNC_ADDRESS;
	int counter = 0;
	bool blocker = false;
}

namespace intern::TIME {
	TIME_FUNC function = NULL;
	TIME_FUNC dFunction = (TIME_FUNC)OFFSETS::TIME_FUNC_ADDRESS;
	bool blocker = false;
}

namespace intern::ENTITY {
	ENTITY_FUNC function = NULL;
	ENTITY_FUNC dFunction = (ENTITY_FUNC)OFFSETS::ENTITY_FUNC_ADDRESS;
	std::set<uintptr_t> entity_list;
}
