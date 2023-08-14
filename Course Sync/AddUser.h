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
	static HWND addFirstNameLabel;
	static HWND addFirstNameTextBox;
public: 
	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
	static HWND roleComboBox;
	static int selectedRole;
	static void setSelectedRole(int role) {
		selectedRole = role;
	}
};