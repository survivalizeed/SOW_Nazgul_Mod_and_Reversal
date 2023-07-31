#pragma once

#define input(key)\
GetAsyncKeyState(key) & 0x8000

#define msg(msg, type)\
MessageBoxA(NULL, msg, "NAZGUL - survivalizeed", type)

namespace intern::GLOW {
	inline char __fastcall detour(__int64 a1, __int64 a2) {
		if (input(global::xray_key)) {
			if (counter++ % 2 == 0) {
				blocker = true;
				return 1;
			}
			blocker = false;
		}
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
		entity_list.insert((uintptr_t)(*(uintptr_t*)(a1 + 40) + 2940));
		return function(a1);
	}
}
namespace intern::ELVENRUNFLAG {
	inline char __fastcall detour(__int64 a1, __int64 a2) {
		if(allow)
			flags.insert((uintptr_t)(a1 + 8));
		return function(a1, a2);
	}
}
namespace intern::MIGHT {
	inline __int64 __fastcall detour(__int64 a1, float a2, char a3) {
		auto ret = function(a1, a2, a3);
		if (blocker) *(float*)(a1 + 720) = 1000.f;
		return ret;
	}
}

namespace intern::FOCUS {
	inline void __fastcall detour(__int64 a1, float a2) {
		function(a1, a2);
		if (blocker) *(float*)(a1 + 572) = 1000.f;
	}
}

inline void hookFunctions() {
	using namespace intern;

	init_offsets();

	if (MH_CreateHook((void**)GLOW::dFunction, &GLOW::detour, (void**)&GLOW::function) != MH_OK) {
		msg("Unable to create Hook: GLOW", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)TIME::dFunction, &TIME::detour, (void**)&TIME::function) != MH_OK) {
		msg("Unable to create Hook: TIME", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)ENTITY::dFunction, &ENTITY::detour, (void**)&ENTITY::function) != MH_OK) {
		msg("Unable to create Hook: ENTITY", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)ELVENRUNFLAG::dFunction, &ELVENRUNFLAG::detour, (void**)&ELVENRUNFLAG::function) != MH_OK) {
		msg("Unable to create Hook: ELVENRUNFLAG", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)MIGHT::dFunction, &MIGHT::detour, (void**)&MIGHT::function) != MH_OK) {
		msg("Unable to create Hook: MIGHT", MB_ICONERROR);
	}
	if (MH_CreateHook((void**)FOCUS::dFunction, &FOCUS::detour, (void**)&FOCUS::function) != MH_OK) {
		msg("Unable to create Hook: FOCUS", MB_ICONERROR);
	}
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		msg("Failed to enable hooks", MB_ICONERROR);
	}
}