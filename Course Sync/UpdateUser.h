#pragma once

#include <Windows.h>

class UpdateUser
{
private:
	static HWND logoutButton;
	static HWND getUserIDLabel;
	static HWND getUserIDTextBox;
	static HWND fieldComboBox;
	static HWND inputLabel;
	static HWND inputTextBox;
	static HWND submitUserUpdateButton;
public:
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
};