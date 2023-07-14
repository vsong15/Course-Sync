#pragma once

#include <Windows.h>

class Home
{
public:
    static void Display(HWND hWnd);
    static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
};
