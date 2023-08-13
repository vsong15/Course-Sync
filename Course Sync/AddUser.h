#pragma once

#include <Windows.h>

class AddUser
{
private:
	static HWND logoutButton;
	static HWND addUsernameLabel;
	static HWND addUsernameTextBox;
public: 
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
};