#include "pch.h"

namespace intern::OFFSETS {
	uintptr_t BASE = (uintptr_t)GetModuleHandleA(NULL);
	uintptr_t GLOW_FUNC_ADDRESS;
	uintptr_t TIME_FUNC_ADDRESS;
	uintptr_t ENTITY_FUNC_ADDRESS;
	uintptr_t ELVENRUNFLAG_FUNC_ADDRESS;
	uintptr_t MIGHT_FUNC_ADDRESS;
	uintptr_t FOCUS_FUNC_ADDRESS;

	uintptr_t CAMERA_ADDRESS = BASE + 0x273B390;
	uintptr_t PLAYER_BASE_ADDRESS = BASE + 0x26FFB70;
	std::vector<unsigned int> PLAYER_OFFSETS = { 0x28, 0x468, 0x20, 0x8, 0x38, 0x90, 0xAC };
	uintptr_t TIME_FUNC_PARAM2_BASE_ADDRESS = BASE + 0x2A61130;
	std::vector<unsigned int> TIME_FUNC_PARAM2_OFFSETS = { 0xC8, 0x10, 0x50, 0x40, 0xE8, 0x38, 0xB58 };
	uintptr_t IS_MENU_PRESENT = BASE + 0x2A276F0;
}

namespace intern::GLOW {
	GLOW_FUNC function = NULL;
	GLOW_FUNC dFunction;
	int counter = 0;
	bool blocker = false;
}

namespace intern::TIME {
	TIME_FUNC function = NULL;
	TIME_FUNC dFunction;
	bool blocker = false;
}

namespace intern::ENTITY {
	ENTITY_FUNC function = NULL;
	ENTITY_FUNC dFunction;
	std::set<uintptr_t> entity_list;
}

namespace intern::ELVENRUNFLAG {
	ELVENRUNFLAG_FUNC function = NULL;
	ELVENRUNFLAG_FUNC dFunction;
	std::set<uintptr_t> flags;
	std::atomic<bool> allow = true;
}

namespace intern::MIGHT {
	MIGHT_FUNC function = NULL;
	MIGHT_FUNC dFunction;
	bool blocker = false;
}

namespace intern::FOCUS {
	FOCUS_FUNC function = NULL;
	FOCUS_FUNC dFunction;
	bool blocker = false;
}

namespace global {
	int xray_key = 0;
}