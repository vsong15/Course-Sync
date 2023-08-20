#pragma once

#include <Windows.h>
#include <string>

class UpdateUser
{
private:
	static HWND logoutButton;
	static HWND getUserIDLabel;
	static HWND getUserIDTextBox;
	static HWND inputLabel;
	static HWND inputTextBox;
	static HWND submitUserUpdateButton;
	static std::wstring field;
public:
	static HWND fieldComboBox;

	static void Display(HWND hWnd);
	static void DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height);
	static void DestroyControls();
	static std::wstring GetUpdateUserID();
	static std::wstring GetUpdateUserInput();
	static void setSelectedField(std::wstring selectedField) {
		field = selectedField;
	}
	static std::wstring getSelectedField() {
		return field;
	}
};