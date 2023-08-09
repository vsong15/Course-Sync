#pragma once

#include <Windows.h>

class Admin 
{
private: 
	static HWND logoutButton;
	static HWND dashboardButton;
	static HWND addUserButton;
	static int currentUserId;
public: 
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();

	static void SetCurrentUserId(int userId) {
		currentUserId = userId;
	}

	static int GetCurrentUserId() {
		return currentUserId;
	}
};