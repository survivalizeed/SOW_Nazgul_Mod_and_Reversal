#pragma once


#define input(key)\
GetAsyncKeyState(key) & 0x8000

#define msg(msg, type)\
MessageBoxA(NULL, msg, "NAZGUL - survivalizeed", type)

namespace intern::OFFSETS {
	extern uintptr_t GLOW_FUNC_ADDRESS; // 8 bytes
	extern uintptr_t TIME_FUNC_ADDRESS; // 8 bytes
	extern uintptr_t ENTITY_FUNC_ADDRESS; // 8 bytes
	extern uintptr_t ELVENRUNFLAG_FUNC_ADDRESS; // 8 bytes
	extern uintptr_t MIGHT_FUNC_ADDRESS; // 8 bytes
	extern uintptr_t FOCUS_FUNC_ADDRESS; // 8 bytes

	extern uintptr_t CAMERA_ADDRESS; // 8 bytes
	extern uintptr_t PLAYER_BASE_ADDRESS; // 8 bytes
	extern std::vector<unsigned int> PLAYER_OFFSETS; // Result: 12 bytes
	extern uintptr_t TIME_FUNC_PARAM2_BASE_ADDRESS; // 8 bytes
	extern std::vector<unsigned int> TIME_FUNC_PARAM2_OFFSETS; // Result: 8 bytes
	extern uintptr_t IS_MENU_PRESENT; // 1 byte
}

namespace intern::GLOW {
	typedef char(__fastcall* GLOW_FUNC)(__int64, __int64);
	extern GLOW_FUNC function;
	extern GLOW_FUNC dFunction;
	extern int counter;
	extern bool blocker;
}

namespace intern::TIME {
	typedef __int64(__fastcall* TIME_FUNC)(__int64, __int64);
	extern TIME_FUNC function;
	extern TIME_FUNC dFunction;
	extern bool blocker;
}

namespace intern::ENTITY {
	typedef double(__fastcall* ENTITY_FUNC)(__int64);
	extern ENTITY_FUNC function;
	extern ENTITY_FUNC dFunction;
	extern std::set<uintptr_t> entity_list;
}

namespace intern::ELVENRUNFLAG {
	typedef char(__fastcall* ELVENRUNFLAG_FUNC)(__int64, __int64);
	extern ELVENRUNFLAG_FUNC function;
	extern ELVENRUNFLAG_FUNC dFunction;
	extern std::set<uintptr_t> flags;
	extern std::atomic<bool> allow;
}

namespace intern::MIGHT {
	typedef __int64(__fastcall* MIGHT_FUNC)(__int64, float, char);
	extern MIGHT_FUNC function;
	extern MIGHT_FUNC dFunction;
	extern bool blocker;
}

namespace intern::FOCUS {
	typedef void(__fastcall* FOCUS_FUNC)(__int64, float);
	extern FOCUS_FUNC function;
	extern FOCUS_FUNC dFunction;
	extern bool blocker;
}

namespace global {
	extern int xray_key;
}

namespace intern::TYPES {
	// Math is "wrong" on purpose
	struct Vec3_XZ {
		float x, y, z;
		void normalize() {
			float tmp = sqrtf(x * x + z * z);
			if (tmp == 0.f) tmp = 1.f;
			x /= tmp;
			z /= tmp;
		}
		Vec3_XZ operator+(Vec3_XZ other) {
			return Vec3_XZ{ x + other.x, y, z + other.z };
		}
		Vec3_XZ operator-(Vec3_XZ other) {
			return Vec3_XZ{ x - other.x, y, z - other.z };
		}
		Vec3_XZ operator*(float val) {
			return Vec3_XZ{ x * val, y, z * val };
		}
		float len() {
			return sqrtf(x * x + z * z);
		}
	};

	struct Vec3 {
		float x, y, z;
	};
}

namespace intern::FUNCTIONS {
	using namespace intern::TYPES;

	inline void patch(void* addr, std::vector<BYTE> bytes) {
		DWORD oldProtect;
		VirtualProtect(addr, bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
		for (int i = 0; i < bytes.size(); ++i) {
			*((BYTE*)addr + i) = bytes[i];
		}
		VirtualProtect(addr, bytes.size(), oldProtect, &oldProtect);
	}

	inline std::vector<BYTE> nop(int count) {
		std::vector<BYTE> v;
		for (int i = 0; i < count; ++i) v.push_back(0x90);
		return v;
	}

	inline bool isBadReadPtr(void* p)
	{
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		if (::VirtualQuery(p, &mbi, sizeof(mbi)))
		{
			DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool b = !(mbi.Protect & mask);
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;
			return b;
		}
		return true;
	}

	inline uintptr_t calcAddS(uintptr_t ptr, std::vector<unsigned int> offsets, bool& valid)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size() - 1; ++i) {
			if (isBadReadPtr((void*)(addr + offsets[i]))) {
				valid = false;
				return 0;
			}
			addr = *(uintptr_t*)(addr + offsets[i]);
		}
		valid = true;
		return addr + offsets[offsets.size() - 1];
	}

	inline uintptr_t scan_pattern(const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<char>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back('\?');
				}
				else
				{
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(NULL), &info, sizeof(MODULEINFO));
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		uintptr_t sizeOfImage = (uintptr_t)info.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);

		uintptr_t patternLength = patternBytes.size();
		auto data = patternBytes.data();

		for (uintptr_t i = 0; i < sizeOfImage - patternLength; i++)
		{
			bool found = true;
			for (uintptr_t j = 0; j < patternLength; j++)
			{
				char a = '\?';
				char b = *(char*)(base + i + j);
				found &= data[j] == a || data[j] == b;
			}
			if (found)
			{
				return base + i;
			}
		}
		return NULL;
	}

	inline float calc_vec_dist(const Vec3* const v1, const Vec3* const v2) {
		return sqrtf(powf(v1->x - v2->x, 2) + powf(v1->y - v2->y, 2) + powf(v1->z - v2->z, 2));
	}

	inline Vec3 calc_vec_dir(const Vec3* const v1, const Vec3* const v2, float null_faktor) {
		auto dist = calc_vec_dist(v1, v2);
		if (dist < null_faktor) return Vec3{ 0.f,0.f,0.f };
		return Vec3{ (v2->x - v1->x) / dist, (v2->y - v1->y) / dist, (v2->z - v1->z) / dist };
	}

	inline float calc_angle_player_camera_entity_horizontal(const Vec3* const v1, const Vec3* const v2) {
		constexpr auto pi = 3.14159f;
		Vec3* camera = (Vec3*)OFFSETS::CAMERA_ADDRESS;
		Vec3 camv1_vec = calc_vec_dir(camera, v1, 5.f);
		camv1_vec.x *= 1000.f;
		camv1_vec.y *= 1000.f;
		camv1_vec.z *= 1000.f;
		Vec3 v1v2_vec{ v2->x - v1->x, v2->y - v1->y, v2->z - v1->z };
		float angle_rad = atan2(camv1_vec.z, camv1_vec.x) - atan2(v1v2_vec.z, v1v2_vec.x);
		if (angle_rad < 0)
			angle_rad += 2 * pi;
		float angle_deg = angle_rad * 180 / pi;
		if (angle_deg < 0)
			angle_deg += 360;
		return angle_deg;
	}

	inline Vec3 abs_vec_diff(const Vec3* const v1, const Vec3* const v2) {
		return Vec3{ abs(v1->x - v2->x),abs(v1->y - v2->y) ,abs(v1->z - v2->z) };
	}
}

