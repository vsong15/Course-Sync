#pragma once

#include <Windows.h>

class AddUser
{
private:
	static HWND logoutButton;
	static HWND addUsernameLabel;
	static HWND addUsernameTextBox;
	static HWND addPasswordLabel;
	static HWND addPasswordTextBox;
public: 
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
};