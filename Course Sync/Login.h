#pragma once

#include <Windows.h>

class Login
{
private:
    static HWND usernameTextBox; // Username input field window handle
    static HWND passwordTextBox; // Password input field window handle
    static HWND loginButton;     // Login button window handle

public:
    static void Display(HWND hWnd);
    static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
    static void DestroyControls();
};
