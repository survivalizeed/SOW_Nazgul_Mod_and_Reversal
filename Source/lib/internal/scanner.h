#pragma once


inline bool init_offsets() {
	using namespace intern;
	OFFSETS::GLOW_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"( 48 83 ec ? 4c 8b c2 e8 ? ? ? ? b0 ? 48 83 c4 ? c3 cc 48 89 5c 24 ? 48 89 6c 24 ? 48 89 74 
																				24 ? 57 41 56 41 57 48 83 ec ? 48 8b f9 4d 8b f0 48 8b 49 ? 48 8b f1 48 2b 37 48 c1 fe ? 3b 77 ? 
																				73 ? 48 85 c9 74 ? 49 8b 00 49 83 20 ? 48 89 01 48 83 47 ? ? 48 8b 5c 24 ? 48 8b 6c 24 ? 48 8b 
																				74 24 ? 48 83 c4 ? 41 5f 41 5e 5f c3 8b ee 83 e5 ? 83 c5 ? 8b dd c1 e3 ? e8 ? ? ? ? 48 8b c8 41 
																				b8 ? ? ? ? 8b d3 e8 ? ? ? ? 4c 8b 4f ? 4c 8b f8 48 8b 17 4c 8b c0 49 3b d1 0f 85 ? ? ? ? 49 8d 58 ? 4d 
																				85 c0 74 ? 49 8b 06 49 83 26 ? 49 89 00 48 8b 0f 8b d6 e8 ? ? ? ? 80 7f ? ? 0f 85 ? ? ? ? 4c 89 3f 48 89 
																				5f ? 89 6f ? c6 47 ? ? e9 ? ? ? ? 48 8b c4)");
	
	OFFSETS::TIME_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"( 48 89 5c 24 ? 48 89 74 24 ? 57 48 83 ec ? 80 3d ? ? ? ? ? 48 8b fa 48 8b 35 ? ? ? ? 0f 85 ? ? ? ?
																				45 33 c9 4c 8b c7 48 8b ce 41 8d 51 ? 48 8b 5c 24 ? 48 8b 74 24 ? 48 83 
																				c4 ? 5f e9 ? ? ? ? cc 48 83 ec ? 4c 8b d1)");

	OFFSETS::ENTITY_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"( 48 8b c4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 
																				  ec ? ? ? ? 0f 29 70 ? 48 8b d9)");

	OFFSETS::ELVENRUNFLAG_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"(40 53 48 83 ec ? 48 8b c2 48 8b d9 48 8b 11)");

	OFFSETS::MIGHT_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"( 48 8b c4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 48 8d 68 ? 48 81 ec ? ? ? ? 
																				 0f 29 70 ? 0f 57 f6)");

	OFFSETS::FOCUS_FUNC_ADDRESS = (uintptr_t)intern::FUNCTIONS::scan_pattern(R"( 48 83 ec ? 80 3d ? ? ? ? ? 4c 8b d1 0f 85)");

	GLOW::dFunction = (GLOW::GLOW_FUNC)OFFSETS::GLOW_FUNC_ADDRESS;
	TIME::dFunction = (TIME::TIME_FUNC)OFFSETS::TIME_FUNC_ADDRESS;
	ENTITY::dFunction = (ENTITY::ENTITY_FUNC)OFFSETS::ENTITY_FUNC_ADDRESS;
	ELVENRUNFLAG::dFunction = (ELVENRUNFLAG::ELVENRUNFLAG_FUNC)OFFSETS::ELVENRUNFLAG_FUNC_ADDRESS;
	MIGHT::dFunction = (MIGHT::MIGHT_FUNC)OFFSETS::MIGHT_FUNC_ADDRESS;
	FOCUS::dFunction = (FOCUS::FOCUS_FUNC)OFFSETS::FOCUS_FUNC_ADDRESS;
	return true;
}