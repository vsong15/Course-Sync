#pragma once

#include <Windows.h>

class Button {
public:
    static void ButtonClicked(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HINSTANCE hInst, int activeWindow);
};