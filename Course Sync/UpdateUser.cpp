#include "UpdateUser.h"
#include <Winuser.h>
#include "Constants.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

HWND UpdateUser::logoutButton = nullptr;
HWND UpdateUser::getUserIDLabel = nullptr;
HWND UpdateUser::getUserIDTextBox = nullptr;
HWND UpdateUser::fieldComboBox = nullptr;
HWND UpdateUser::inputLabel = nullptr;
HWND UpdateUser::inputTextBox = nullptr;
HWND UpdateUser::submitUserUpdateButton = nullptr;

void UpdateUser::Display(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // Set the background color
    HBRUSH hBrush = CreateSolidBrush(RGB(248, 248, 255)); // #A7C5E5
    FillRect(hdc, &ps.rcPaint, hBrush);

    // Set the text color
    SetTextColor(hdc, RGB(53, 99, 158)); // #35639E

    // Set the text background color to match the main window background color
    SetBkColor(hdc, RGB(248, 248, 255));

    // Get the client area dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Calculate the section dimensions
    int navBarWidth = 150; // Width of the navigation bar
    int sectionWidth = width - navBarWidth; // Width of the subsection excluding the navigation bar
    int sectionHeight = height; // Adjust the height as needed

    int cornerRadius = 10; // Adjust the corner radius as needed
    int textTopMargin = 5; // Adjust the top margin for text as needed

    // Create a subsection background rectangle
    RECT subsectionRect = { navBarWidth, 50, width, sectionHeight }; // Adjust the left coordinate to account for the navigation bar
    HBRUSH hSubsectionBrush = CreateSolidBrush(RGB(230, 230, 230)); // Color for the subsection background
    SelectObject(hdc, hSubsectionBrush);
    RoundRect(hdc, subsectionRect.left, subsectionRect.top, subsectionRect.right, subsectionRect.bottom, cornerRadius, cornerRadius);

    HBRUSH hDarkBrush = CreateSolidBrush(RGB(12, 42, 51));
    RECT navBarRect = { 0, 0, 150, height }; // Adjust the width as needed
    FillRect(hdc, &navBarRect, hDarkBrush);

    RECT logoutButtonRect;
    int buttonWidth = 120; // Adjust the button width as needed
    int buttonHeight = 40; // Adjust the button height as needed
    int buttonX = 14;      // Adjust the X position as needed
    int buttonY = height - buttonHeight - 10; // Adjust the Y position as needed

    SetRect(&logoutButtonRect, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Update or create the login button
    if (logoutButton != NULL)
    {
        SetWindowPos(logoutButton, NULL, logoutButtonRect.left, logoutButtonRect.top, logoutButtonRect.right - logoutButtonRect.left, logoutButtonRect.bottom - logoutButtonRect.top, SWP_NOZORDER);
    }
    else
    {
        logoutButton = CreateWindowW(
            L"BUTTON",                   // Predefined class; Unicode assumed
            L"Logout",                    // Button text
            WS_CHILD | WS_VISIBLE,       // Styles
            logoutButtonRect.left,             // x position
            logoutButtonRect.top,              // y position
            logoutButtonRect.right - logoutButtonRect.left,    // Button width
            logoutButtonRect.bottom - logoutButtonRect.top,    // Button height
            hWnd,                      // Parent window
            (HMENU)ID_BUTTON_LOGOUT,                        // No menu
            NULL,                        // Instance handle
            NULL                         // Additional application data
        );
    }

    // Update or create the UserID input field and label
    RECT getUserIDRect;
    int userIDLabelWidth = 100; // Width of the "UserID:" label
    int userIDInputWidth = 200; // Width of the input field
    int userIDInputHeight = 25; // Height of the input field

    // Center the input field below the "Add User" text
    int centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    int centerY = subsectionRect.top + 40; // Adjust this value as needed

    // Calculate the left coordinate for the input field
    int inputLeft = centerX - (userIDLabelWidth + userIDInputWidth) / 2;

    SetRect(&getUserIDRect, inputLeft, centerY, inputLeft + userIDLabelWidth + userIDInputWidth, centerY + userIDInputHeight);

    if (getUserIDTextBox != NULL)
    {
        SetWindowPos(getUserIDLabel, NULL, getUserIDRect.left, getUserIDRect.top, userIDLabelWidth, getUserIDRect.bottom - getUserIDRect.top, SWP_NOZORDER);
        SetWindowPos(getUserIDTextBox, NULL, getUserIDRect.left + userIDLabelWidth, getUserIDRect.top, getUserIDRect.right - getUserIDRect.left - userIDLabelWidth, getUserIDRect.bottom - getUserIDRect.top, SWP_NOZORDER);
    }
    else
    {
        // Create the "UserID:" label with improved visual appearance
        getUserIDLabel = CreateWindowW(L"STATIC", L"UserID:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, getUserIDRect.left, getUserIDRect.top, userIDLabelWidth, userIDInputHeight, hWnd, NULL, NULL, NULL);

        // Create the UserID input field with improved visual appearance and resizable style
        getUserIDTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            getUserIDRect.left + userIDLabelWidth, getUserIDRect.top, userIDInputWidth, userIDInputHeight,
            hWnd, NULL, NULL, NULL);

        // Set font for the input field
        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(getUserIDLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(getUserIDTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        // Set light gray background color for the input field
        SetWindowLongPtr(getUserIDTextBox, GWL_EXSTYLE, GetWindowLongPtr(getUserIDTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(getUserIDTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT fieldComboRect;
    // Calculate the center position for the field combo box
    int comboWidth = 200; // Width of the combo box
    int comboHeight = 160; // Height of the combo box
    int comboLeft = centerX - comboWidth / 2;
    int comboTop = centerY + 140 - comboHeight / 2; // Center vertically based on centerY

    SetRect(&fieldComboRect, comboLeft, comboTop, comboLeft + comboWidth, comboTop + comboHeight);

    if (fieldComboBox != NULL) {
        SetWindowPos(fieldComboBox, NULL, fieldComboRect.left, fieldComboRect.top, comboWidth, comboHeight, SWP_NOZORDER);
    }
    else {
        fieldComboBox = CreateWindowW(
            L"COMBOBOX",                    // Predefined class; Unicode assumed
            L"SELECT A FIELD",                           // No text needed initially
            WS_CHILD | WS_VISIBLE | CBS_SIMPLE, // Styles
            fieldComboRect.left,             // x position
            fieldComboRect.top,              // y position
            comboWidth,                     // Combo box width
            comboHeight,                    // Combo box height
            hWnd,                           // Parent window
            (HMENU)ID_COMBOBOX_FIELD,       // Control identifier
            NULL,                           // Instance handle
            NULL                            // Additional application data
        );

        // Add field options to the combo box
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"Username");
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"Password");
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"Role");
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"First Name");
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"Last Name");
        SendMessage(fieldComboBox, CB_ADDSTRING, 0, (LPARAM)L"Email");

        // Create a font with size 20
        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

        // Set the font to the combo box
        SendMessage(fieldComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    // Update or create the input field and label
    RECT inputRect;
    int inputLabelWidth = 100; // Width of the label
    int inputFieldWidth = 200; // Width of the input field
    int inputFieldHeight = 25; // Height of the input field

    // Center the input field below the "Add User" text
    centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    centerY = subsectionRect.top + 280; // Adjust this value as needed

    // Calculate the left coordinate for the input field
    inputLeft = centerX - (inputLabelWidth + inputFieldWidth) / 2;

    SetRect(&inputRect, inputLeft, centerY, inputLeft + inputLabelWidth + inputFieldWidth, centerY + inputFieldHeight);

    if (inputTextBox != NULL)
    {
        SetWindowPos(inputLabel, NULL, inputRect.left, inputRect.top, inputLabelWidth, inputRect.bottom - inputRect.top, SWP_NOZORDER);
        SetWindowPos(inputTextBox, NULL, inputRect.left + inputLabelWidth, inputRect.top, inputRect.right - inputRect.left - inputLabelWidth, inputRect.bottom - inputRect.top, SWP_NOZORDER);
    }
    else
    {
        // Create the label with improved visual appearance
        inputLabel = CreateWindowW(L"STATIC", L"Input:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, inputRect.left, inputRect.top, inputLabelWidth, inputFieldHeight, hWnd, NULL, NULL, NULL);

        // Create the input field with improved visual appearance and resizable style
        inputTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            inputRect.left + inputLabelWidth, inputRect.top, inputFieldWidth, inputFieldHeight,
            hWnd, NULL, NULL, NULL);

        // Set font for the input field
        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(inputLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(inputTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        // Set light gray background color for the input field
        SetWindowLongPtr(inputTextBox, GWL_EXSTYLE, GetWindowLongPtr(inputTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(inputTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT submitUserUpdateButtonRect; // Change variable name to submitUserUpdateButtonRect
    int submitUserUpdateButtonWidth = 100; // Change variable name to submitUserUpdateButtonWidth
    int submitUserUpdateButtonHeight = 30; // Change variable name to submitUserUpdateButtonHeight
    int submitUserUpdateButtonX = (subsectionRect.left + subsectionRect.right - submitUserUpdateButtonWidth) / 2;
    int submitUserUpdateButtonY = 400;

    SetRect(&submitUserUpdateButtonRect, submitUserUpdateButtonX, submitUserUpdateButtonY, submitUserUpdateButtonX + submitUserUpdateButtonWidth, submitUserUpdateButtonY + submitUserUpdateButtonHeight);

    // Update or create the submitUserUpdateButton button
    if (submitUserUpdateButton != NULL) {
        SetWindowPos(submitUserUpdateButton, NULL, submitUserUpdateButtonRect.left, submitUserUpdateButtonRect.top, submitUserUpdateButtonRect.right - submitUserUpdateButtonRect.left, submitUserUpdateButtonRect.bottom - submitUserUpdateButtonRect.top, SWP_NOZORDER);
    }
    else {
        submitUserUpdateButton = CreateWindowW(
            L"BUTTON",                              // Predefined class; Unicode assumed
            L"Submit",                              // Button text
            WS_CHILD | WS_VISIBLE,                  // Styles
            submitUserUpdateButtonRect.left,        // x position
            submitUserUpdateButtonRect.top,         // y position
            submitUserUpdateButtonRect.right - submitUserUpdateButtonRect.left,  // Button width
            submitUserUpdateButtonRect.bottom - submitUserUpdateButtonRect.top,  // Button height
            hWnd,                                   // Parent window
            (HMENU)ID_BUTTON_SUBMIT_USER_UPDATE,    // No menu
            NULL,                                   // Instance handle
            NULL                                    // Additional application data
        );
    }

    // Calculate the center position for the logo
    int logoWidth = 140; // Set the desired width of the logo
    int logoHeight = 30; // Set the desired height of the logo
    int logoLeftMargin = 4;
    int logoTopMargin = 10;

    // Load the image using GDI+
    Image image(L"courseSyncLogo.bmp");

    // Draw the logo
    Graphics graphics(hdc);
    graphics.DrawImage(&image, logoLeftMargin, logoTopMargin, logoWidth, logoHeight);

    HFONT hWelcomeFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hWelcomeFont);
    DrawTextCenter(hdc, L"Update User", 5, width + 150, height);
}

void UpdateUser::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height) {
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void UpdateUser::DestroyControls() {
    if (logoutButton != nullptr) {
        DestroyWindow(logoutButton);
        logoutButton = nullptr;
    }
    if (getUserIDLabel != nullptr) {
        DestroyWindow(getUserIDLabel);
        getUserIDLabel = nullptr;
    }
    if (getUserIDTextBox != nullptr) {
        DestroyWindow(getUserIDTextBox);
        getUserIDTextBox = nullptr;
    }
    if (fieldComboBox != nullptr) {
        DestroyWindow(fieldComboBox);
        fieldComboBox = nullptr;
    }
    if (inputLabel != nullptr) {
        DestroyWindow(inputLabel);
        inputLabel = nullptr;
    }
    if (inputTextBox != nullptr) {
        DestroyWindow(inputTextBox);
        inputTextBox = nullptr;
    }
    if (submitUserUpdateButton != nullptr) {
        DestroyWindow(submitUserUpdateButton);
        submitUserUpdateButton = nullptr;
    }
}