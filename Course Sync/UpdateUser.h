#pragma once

#include <Windows.h>

class UpdateUser
{
private:
	static HWND logoutButton;
	static HWND dashboardButton;
	static HWND addUserButton;
public:
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
};