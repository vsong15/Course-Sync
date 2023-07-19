#pragma once

#include <Windows.h>
#include <string>

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

    // Add this function to get the entered username and password
    static std::wstring GetUsername();
    static std::wstring GetPassword();
};
