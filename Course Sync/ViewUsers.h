#pragma once

#include <Windows.h>

class ViewUsers {
private: 
	static HWND logoutButton;
public:
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
	static void PopulateTable(HWND hWnd, int sectionWidth, int sectionHeight);
};