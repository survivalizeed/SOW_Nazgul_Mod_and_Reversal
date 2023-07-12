#pragma once

namespace overlay {

    inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            exit(0);
            break;
        case WM_DESTROY:
            std::cout << "postquit";
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }

    inline void wndCreate(HWND* hwnd, const std::wstring& name, int params = WS_POPUP | WS_VISIBLE)
    {
        auto win = [&](HWND* hwnd)
        {
            MSG msg;
            WNDCLASSEX wc = {};
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WndProc;
            wc.hInstance = GetModuleHandle(0);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = L"CLASS";
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            RegisterClassEx(&wc);
            *hwnd = CreateWindowEx(
                WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
                L"CLASS",
                name.c_str(),
                WS_EX_TOPMOST | WS_POPUP,
                0,
                0,
                1920,
                1080,
                nullptr,
                nullptr,
                GetModuleHandle(0),
                nullptr);
            const MARGINS margin = { -1 };
            SetLayeredWindowAttributes(*hwnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);
            DwmExtendFrameIntoClientArea(*hwnd, &margin);

            ShowWindow(*hwnd, SW_SHOW);
            UpdateWindow(*hwnd);
            while (GetMessage(&msg, NULL, 0, 0) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        };

        std::thread window_thread(win, hwnd);
        window_thread.detach();
        while (*hwnd == NULL) {
            Sleep(1);
        }
        SetParent(*hwnd, NULL);
    }

}