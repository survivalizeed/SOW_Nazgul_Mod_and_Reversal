// pch.h: Dies ist eine vorkompilierte Headerdatei.
// Die unten aufgeführten Dateien werden nur einmal kompiliert, um die Buildleistung für zukünftige Builds zu verbessern.
// Dies wirkt sich auch auf die IntelliSense-Leistung aus, Codevervollständigung und viele Features zum Durchsuchen von Code eingeschlossen.
// Die hier aufgeführten Dateien werden jedoch ALLE neu kompiliert, wenn mindestens eine davon zwischen den Builds aktualisiert wird.
// Fügen Sie hier keine Dateien hinzu, die häufig aktualisiert werden sollen, da sich so der Leistungsvorteil ins Gegenteil verkehrt.

#ifndef PCH_H
#define PCH_H

// Fügen Sie hier Header hinzu, die vorkompiliert werden sollen.
#include "framework.h"
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <Psapi.h>
#include <vector>
#include <MinHook.h>
#include <fstream>
#include <math.h>
#include <set>
#include <thread>
#include <dwmapi.h>
#include <dwrite.h>
#include <d2d1.h>
#include <wincodec.h>
#include <comdef.h>
#include "lib/ini.h"
#include "lib/internal/defines.h"
#include "lib/internal/detour.h"
#include "lib/modification.h"
#include "lib/overlay/window.h"
#include "lib/overlay/directX.h"
#include "lib/overlay/overlay.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwmapi.lib")

#endif //PCH_H
