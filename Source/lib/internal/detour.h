#pragma once

#include "../../pch.h"

#define msg(msg, type)\
MessageBoxA(NULL, msg, "NAZGUL - survivalizeed", type)

namespace intern::GLOW {
	inline char __fastcall detour(__int64 a1, __int64 a2) {
		if (counter++ % 2 == 0) {
			blocker = true;
			return 1;
		}
		blocker = false;
		return function(a1, a2);
	}
}
namespace intern::TIME {
	inline __int64 __fastcall detour(__int64 a1, __int64 a2) {
		if (blocker) return 0;
		return function(a1, a2);
	}
}
namespace intern::ENTITY {
	inline double __fastcall detour(__int64 a1) {
		uintptr_t* ptr = (uintptr_t*)(*(uint64_t*)(a1 + 40) + 2940);
		entity_list.insert((uintptr_t)ptr);
		return function(a1);
	}
}

inline void hookFunctions() {
	using namespace intern;
	if (MH_CreateHook((void**)GLOW::dFunction, &GLOW::detour, (void**)&GLOW::function) != MH_OK) {
		msg("Unable to create Hook (sub_0x1405902AC)", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)TIME::dFunction, &TIME::detour, (void**)&TIME::function) != MH_OK) {
		msg("Unable to create Hook (sub_0x1407A35D8)", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)ENTITY::dFunction, &ENTITY::detour, (void**)&ENTITY::function) != MH_OK) {
		msg("Unable to create Hook (sub_0x14024FEA0)", MB_ICONERROR);
	}
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		msg("Failed to enable hooks.", MB_ICONERROR);
	}
}