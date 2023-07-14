#pragma once

namespace overlay {
	inline Direct2DRenderer* initialize(HWND& retHWND) {
        Direct2DRenderer* pRenderer = nullptr;
        HWND hwnd = 0;
        HWND targetwindow = 0;
        for (;;) {
            targetwindow = GetTopWindow(GetDesktopWindow());
            do
            {
                if (!IsWindowVisible(targetwindow))
                    continue;

                char titleString[64];
                GetWindowTextA(targetwindow, titleString, 64);
                std::string str(titleString);
                if (str.starts_with("Mittelerde"))
                    goto leave;
                if (str.starts_with("Middle-earth"))
                    goto leave;

            } while (targetwindow = GetWindow(targetwindow, GW_HWNDNEXT));
        }
    leave:;
        wndCreate(&hwnd, L"SOW-OVERLAY");
        pRenderer = new Direct2DRenderer(hwnd);
        std::thread async(rescaler, hwnd, targetwindow, pRenderer);
        async.detach();
        retHWND = hwnd;
        return pRenderer;
	}

    inline void display(Direct2DRenderer* pRenderer) {
        static bool once = false;
        static bool once2 = false;
        static bool once3 = false;
        static bool anyactive = true;

        if (!once) {
            pRenderer->AddImage(L"./plugins/Nazgul/Resource/SOW_SPLASH.jpg", 1463.f, 43.f);
            once = true;
        }

        {
            pRenderer->GetImage(0).active = ((*(BYTE*)intern::OFFSETS::IS_MENU_PRESENT) == 0) ? false : true;
            if (pRenderer->GetImage(0).active)
                once3 = true;
            if (once3 && !pRenderer->GetImage(0).active)
                anyactive = false;
        }

        if (anyactive) {
            pRenderer->Render();
            once2 = false;
        }
        else if (!once2) {
            pRenderer->Clear();
            once2 = true;
        }
    }
}