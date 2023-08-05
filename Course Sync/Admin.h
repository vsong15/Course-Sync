#pragma once

#include <Windows.h>

class Admin 
{
private: 
	static HWND logoutButton;
	static HWND dashboardButton;
public: 
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
};