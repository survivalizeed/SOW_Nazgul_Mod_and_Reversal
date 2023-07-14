#include "pch.h"


void ini_init(float& elven_run_speed, int& elven_run_smtd, int& reload_key, float& dash_max_distance, int& dash_key) {
	mINI::INIFile file("./plugins/Nazgul/Nazgul.ini");
	mINI::INIStructure ini;	
	if (!file.read(ini)) {
		msg("Could not load the ini file. Make sure you placed the ini file in the plugins/Nazgul folder", MB_ICONERROR);
		exit(-1);
	}
	elven_run_speed = std::stof(ini["ELVENRUN"]["elven_run_speed"]);
	elven_run_smtd = std::stoi(ini["ELVENRUN"]["elven_run_slow_motion_time_delay"]);
	reload_key = std::stoi(ini["MISC"]["reload_key"]);
	dash_max_distance = std::stof(ini["DASH"]["dash_max_distance"]);
	dash_key = std::stoi(ini["DASH"]["dash_key"]);
}

void integrity() {
	SHA256 sha;
	std::ifstream file("./plugins/Nazgul/Resource/SOW_SPLASH.jpg", std::ios::binary);
	std::stringstream ss;
	ss << file.rdbuf();
	sha.update(ss.str());
	auto s = sha.toString(sha.digest());
	if (s != "52275bf75a3b5609fcecba218768a1164dc3577adf684b58300fb915bf406023") {
		msg("Please keep the integrity of the /plugins/Nazgul/Resource/SOW_SPLASH.jpg", MB_ICONERROR);
		exit(-1);
	}	
}

DWORD WINAPI MainThread(LPVOID param) {

	if (!std::ifstream("./plugins/Nazgul/sowNZ.sa")) {
		MessageBoxA(NULL, "Introducing a remarkable gift from survivalizeed: a free mod that unveils an entirely new realm of possibilities. This unique creation allows you to assume the role of a genuine Nazgul while preserving the core functionality of the game. Rest assured, this message will appear only once, granting you the extraordinary opportunity to embrace the dark allure of the Nazgul within the game's immersive world.",
			"This Message will only show once!", MB_ICONINFORMATION);
	}
	{
		std::ofstream("./plugins/Nazgul/sowNZ.sa"); 
	}

	using namespace intern::TYPES;
	using namespace intern::FUNCTIONS;
	using namespace intern;

	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	MH_Initialize();
	hookFunctions();

	float elven_run_speed, dash_max_distance;
	int elven_run_smtd, reload_key, dash_key;

	ini_init(elven_run_speed, elven_run_smtd, reload_key, dash_max_distance, dash_key);
	integrity();

	uintptr_t* position = nullptr;
	__int64* time_func_param2 = nullptr;
	float* might = nullptr;

	bool valid;

	HWND window_hwnd;
	overlay::Direct2DRenderer* pRenderer = overlay::initialize(window_hwnd);
	
	for (;;) {
		overlay::display(pRenderer);
		{
			valid = false;
			if (!isBadReadPtr((void*)OFFSETS::PLAYER_BASE_ADDRESS)) {
				position = (uintptr_t*)calcAddS(*(uintptr_t*)(OFFSETS::PLAYER_BASE_ADDRESS), OFFSETS::PLAYER_OFFSETS, valid);
			}
			if (!valid)
				position = nullptr;

			valid = false;
			if (!isBadReadPtr((void*)OFFSETS::TIME_FUNC_PARAM2_BASE_ADDRESS)) {
				time_func_param2 = (__int64*)calcAddS(*(uintptr_t*)OFFSETS::TIME_FUNC_PARAM2_BASE_ADDRESS, OFFSETS::TIME_FUNC_PARAM2_OFFSETS, valid);
			}
			if (!valid)
				time_func_param2 = nullptr;

			if (!isBadReadPtr((void*)OFFSETS::MIGHT_BASE_ADDRESS)) {
				might = (float*)calcAddS(*(uintptr_t*)(OFFSETS::MIGHT_BASE_ADDRESS), OFFSETS::MIGHT_OFFSETS, valid);
			}
			if (!valid)
				might = nullptr;
		}

		modification::modify_elven_run(position, time_func_param2, elven_run_speed, elven_run_smtd);

		modification::modify_might(might);
		
		if (input(dash_key)) {
			modification::modify_entity_tp(position, dash_max_distance);
		}
		if (input(reload_key)) {
			ini_init(elven_run_speed, elven_run_smtd, reload_key, dash_max_distance, dash_key);
			msg("Reloaded ini", MB_ICONINFORMATION);
			Sleep(300);
		}
		//if (input(VK_NUMPAD0)) {
		//	global::eject = true;
		//	break;
		//}
		Sleep(1);
	}
	
	delete pRenderer;
	PostMessage(window_hwnd, WM_DESTROY, 0, 0);
	//FreeConsole();
	Sleep(1000);
	
	MH_DisableHook(MH_ALL_HOOKS);
	FreeLibraryAndExitThread((HMODULE)param, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:	
		CreateThread(NULL, NULL, MainThread, hModule, NULL, NULL);
		break;
	default:
		break;
	}
	return TRUE;
}
