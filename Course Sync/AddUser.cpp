#include "AddUser.h"
#include <Winuser.h>
#include "Constants.h"
#include "CommCtrl.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

HWND AddUser::logoutButton = nullptr;
HWND AddUser::addUsernameLabel = nullptr;
HWND AddUser::addUsernameTextBox = nullptr;
HWND AddUser::addPasswordLabel = nullptr;
HWND AddUser::addPasswordTextBox = nullptr;
HWND AddUser::roleComboBox = nullptr;
HWND AddUser::addFirstNameLabel = nullptr;
HWND AddUser::addFirstNameTextBox = nullptr;
HWND AddUser::addLastNameLabel = nullptr;
HWND AddUser::addLastNameTextBox = nullptr;
HWND AddUser::addEmailLabel = nullptr;
HWND AddUser::addEmailTextBox = nullptr;
HWND AddUser::submitUserButton = nullptr;
int AddUser::selectedRole = -1;

void AddUser::Display(HWND hWnd) {
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
    RECT navBarRect = { 0, 0, navBarWidth, height }; // Width is set to the navigation bar width
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

    // Update or create the username input field and label
    RECT addUsernameRect;
    int usernameLabelWidth = 100; // Width of the "Username:" label
    int usernameInputWidth = 200; // Width of the input field
    int usernameInputHeight = 25; // Height of the input field

    // Center the input field below the "Add User" text
    int centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    int centerY = subsectionRect.top + 40; // Adjust this value as needed

    // Calculate the left coordinate for the input field
    int inputLeft = centerX - (usernameLabelWidth + usernameInputWidth) / 2;

    SetRect(&addUsernameRect, inputLeft, centerY, inputLeft + usernameLabelWidth + usernameInputWidth, centerY + usernameInputHeight);

    if (addUsernameTextBox != NULL)
    {
        SetWindowPos(addUsernameLabel, NULL, addUsernameRect.left, addUsernameRect.top, usernameLabelWidth, addUsernameRect.bottom - addUsernameRect.top, SWP_NOZORDER);
        SetWindowPos(addUsernameTextBox, NULL, addUsernameRect.left + usernameLabelWidth, addUsernameRect.top, addUsernameRect.right - addUsernameRect.left - usernameLabelWidth, addUsernameRect.bottom - addUsernameRect.top, SWP_NOZORDER);
    }
    else
    {
        // Create the "Username:" label with improved visual appearance
        addUsernameLabel = CreateWindowW(L"STATIC", L"Username:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, addUsernameRect.left, addUsernameRect.top, usernameLabelWidth, usernameInputHeight, hWnd, NULL, NULL, NULL);

        // Create the username input field with improved visual appearance and resizable style
        addUsernameTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            addUsernameRect.left + usernameLabelWidth, addUsernameRect.top, usernameInputWidth, usernameInputHeight,
            hWnd, NULL, NULL, NULL);

        // Set font for the input field
        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(addUsernameLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(addUsernameTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        // Set light gray background color for the input field
        SetWindowLongPtr(addUsernameTextBox, GWL_EXSTYLE, GetWindowLongPtr(addUsernameTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(addUsernameTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    // Update or create the username input field and label
    RECT addPasswordRect;
    int passwordLabelWidth = 100; // Width of the "Username:" label
    int passwordInputWidth = 200; // Width of the input field
    int passwordInputHeight = 25; // Height of the input field

    // Center the input field below the "Add User" text
    centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    centerY = subsectionRect.top + 80; // Adjust this value as needed

    // Calculate the left coordinate for the input field
    inputLeft = centerX - (passwordLabelWidth + passwordInputWidth) / 2;

    SetRect(&addPasswordRect, inputLeft, centerY, inputLeft + passwordLabelWidth + passwordInputWidth, centerY + passwordInputHeight);

    if (addPasswordTextBox != NULL)
    {
        SetWindowPos(addPasswordLabel, NULL, addPasswordRect.left, addPasswordRect.top, passwordLabelWidth, addPasswordRect.bottom - addPasswordRect.top, SWP_NOZORDER);
        SetWindowPos(addPasswordTextBox, NULL, addPasswordRect.left + passwordLabelWidth, addPasswordRect.top, addPasswordRect.right - addPasswordRect.left - passwordLabelWidth, addPasswordRect.bottom - addPasswordRect.top, SWP_NOZORDER);
    }
    else
    {
        // Create the "Username:" label with improved visual appearance
        addPasswordLabel = CreateWindowW(L"STATIC", L"Password:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, addPasswordRect.left, addPasswordRect.top, passwordLabelWidth, passwordInputHeight, hWnd, NULL, NULL, NULL);

        // Create the username input field with improved visual appearance and resizable style
        addPasswordTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            addPasswordRect.left + passwordLabelWidth, addPasswordRect.top, passwordInputWidth, passwordInputHeight,
            hWnd, NULL, NULL, NULL);

        // Set font for the input field
        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(addPasswordLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(addPasswordTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        // Set light gray background color for the input field
        SetWindowLongPtr(addPasswordTextBox, GWL_EXSTYLE, GetWindowLongPtr(addPasswordTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(addPasswordTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT roleComboRect;
    // Calculate the center position for the role combo box
    int comboWidth = 200; // Width of the combo box
    int comboHeight = 120; // Height of the combo box
    int comboLeft = centerX - comboWidth / 2;
    int comboTop = centerY + 120 - comboHeight / 2; // Center vertically based on centerY

    SetRect(&roleComboRect, comboLeft, comboTop, comboLeft + comboWidth, comboTop + comboHeight);

    if (roleComboBox != NULL) {
        SetWindowPos(roleComboBox, NULL, roleComboRect.left, roleComboRect.top, comboWidth, comboHeight, SWP_NOZORDER);
    }
    else {
        roleComboBox = CreateWindowW(
            L"COMBOBOX",                    // Predefined class; Unicode assumed
            L"SELECT A ROLE",                           // No text needed initially
            WS_CHILD | WS_VISIBLE | CBS_SIMPLE, // Styles
            roleComboRect.left,             // x position
            roleComboRect.top,              // y position
            comboWidth,                     // Combo box width
            comboHeight,                    // Combo box height
            hWnd,                           // Parent window
            (HMENU)ID_COMBOBOX_ROLE,        // Control identifier
            NULL,                           // Instance handle
            NULL                            // Additional application data
        );

        // Add role options to the combo box
        SendMessage(roleComboBox, CB_ADDSTRING, 0, (LPARAM)L"Administrator");
        SendMessage(roleComboBox, CB_ADDSTRING, 0, (LPARAM)L"Student");
        SendMessage(roleComboBox, CB_ADDSTRING, 0, (LPARAM)L"Faculty");
        SendMessage(roleComboBox, CB_ADDSTRING, 0, (LPARAM)L"Staff");

        // Create a font with size 20
        HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

        // Set the font to the combo box
        SendMessage(roleComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    RECT addFirstNameRect;
    int firstNameLabelWidth = 100;
    int firstNameInputWidth = 200;
    int firstNameInputHeight = 25;

    centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    centerY = subsectionRect.top + 280;

    inputLeft = centerX - (firstNameLabelWidth + firstNameInputWidth) / 2;

    SetRect(&addFirstNameRect, inputLeft, centerY, inputLeft + firstNameLabelWidth + firstNameInputWidth, centerY + firstNameInputHeight);

    if (addFirstNameTextBox != NULL)
    {
        SetWindowPos(addFirstNameLabel, NULL, addFirstNameRect.left, addFirstNameRect.top, firstNameLabelWidth, addFirstNameRect.bottom - addFirstNameRect.top, SWP_NOZORDER);
        SetWindowPos(addFirstNameTextBox, NULL, addFirstNameRect.left + firstNameLabelWidth, addFirstNameRect.top, addFirstNameRect.right - addFirstNameRect.left - firstNameLabelWidth, addFirstNameRect.bottom - addFirstNameRect.top, SWP_NOZORDER);
    }
    else
    {
        addFirstNameLabel = CreateWindowW(L"STATIC", L"First Name:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, addFirstNameRect.left, addFirstNameRect.top, firstNameLabelWidth, firstNameInputHeight, hWnd, NULL, NULL, NULL);

        addFirstNameTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            addFirstNameRect.left + firstNameLabelWidth, addFirstNameRect.top, firstNameInputWidth, firstNameInputHeight,
            hWnd, NULL, NULL, NULL);

        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(addFirstNameLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(addFirstNameTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        SetWindowLongPtr(addFirstNameTextBox, GWL_EXSTYLE, GetWindowLongPtr(addFirstNameTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(addFirstNameTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT addLastNameRect;
    int lastNameLabelWidth = 100;
    int lastNameInputWidth = 200;
    int lastNameInputHeight = 25;

    centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    centerY = subsectionRect.top + 320;

    inputLeft = centerX - (lastNameLabelWidth + lastNameInputWidth) / 2;

    SetRect(&addLastNameRect, inputLeft, centerY, inputLeft + lastNameLabelWidth + lastNameInputWidth, centerY + lastNameInputHeight);

    if (addLastNameTextBox != NULL)
    {
        SetWindowPos(addLastNameLabel, NULL, addLastNameRect.left, addLastNameRect.top, lastNameLabelWidth, addLastNameRect.bottom - addLastNameRect.top, SWP_NOZORDER);
        SetWindowPos(addLastNameTextBox, NULL, addLastNameRect.left + lastNameLabelWidth, addLastNameRect.top, addLastNameRect.right - addLastNameRect.left - lastNameLabelWidth, addLastNameRect.bottom - addLastNameRect.top, SWP_NOZORDER);
    }
    else
    {
        addLastNameLabel = CreateWindowW(L"STATIC", L"Last Name:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, addLastNameRect.left, addLastNameRect.top, lastNameLabelWidth, lastNameInputHeight, hWnd, NULL, NULL, NULL);

        addLastNameTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            addLastNameRect.left + lastNameLabelWidth, addLastNameRect.top, lastNameInputWidth, lastNameInputHeight,
            hWnd, NULL, NULL, NULL);

        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(addLastNameLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(addLastNameTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        SetWindowLongPtr(addLastNameTextBox, GWL_EXSTYLE, GetWindowLongPtr(addLastNameTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(addLastNameTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT addEmailRect;
    int emailLabelWidth = 100;
    int emailInputWidth = 200;
    int emailInputHeight = 25;

    centerX = subsectionRect.left + (subsectionRect.right - subsectionRect.left) / 2;
    centerY = subsectionRect.top + 360;

    inputLeft = centerX - (emailLabelWidth + emailInputWidth) / 2;

    SetRect(&addEmailRect, inputLeft, centerY, inputLeft + emailLabelWidth + emailInputWidth, centerY + emailInputHeight);

    if (addEmailTextBox != NULL)
    {
        SetWindowPos(addEmailLabel, NULL, addEmailRect.left, addEmailRect.top, emailLabelWidth, addEmailRect.bottom - addEmailRect.top, SWP_NOZORDER);
        SetWindowPos(addEmailTextBox, NULL, addEmailRect.left + emailLabelWidth, addEmailRect.top, addEmailRect.right - addEmailRect.left - emailLabelWidth, addEmailRect.bottom - addEmailRect.top, SWP_NOZORDER);
    }
    else
    {
        addEmailLabel = CreateWindowW(L"STATIC", L"Email:", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, addEmailRect.left, addEmailRect.top, emailLabelWidth, emailInputHeight, hWnd, NULL, NULL, NULL);

        addEmailTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
            addEmailRect.left + emailLabelWidth, addEmailRect.top, emailInputWidth, emailInputHeight,
            hWnd, NULL, NULL, NULL);

        HFONT hInputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(addEmailLabel, WM_SETFONT, (WPARAM)hInputFont, TRUE);
        SendMessage(addEmailTextBox, WM_SETFONT, (WPARAM)hInputFont, TRUE);

        SetWindowLongPtr(addEmailTextBox, GWL_EXSTYLE, GetWindowLongPtr(addEmailTextBox, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetLayeredWindowAttributes(addEmailTextBox, RGB(240, 240, 240), 0, LWA_COLORKEY);
    }

    RECT submitUserButtonRect; // Change variable name to submitUserButtonRect
    int submitUserButtonWidth = 100; // Change variable name to submitUserButtonWidth
    int submitUserButtonHeight = 30; // Change variable name to submitUserButtonHeight
    int submitUserButtonX = (subsectionRect.left + subsectionRect.right - submitUserButtonWidth) / 2;
    int submitUserButtonY = 470;

    SetRect(&submitUserButtonRect, submitUserButtonX, submitUserButtonY, submitUserButtonX + submitUserButtonWidth, submitUserButtonY + submitUserButtonHeight);

    // Update or create the submitUserButton button
    if (submitUserButton != NULL) {
        SetWindowPos(submitUserButton, NULL, submitUserButtonRect.left, submitUserButtonRect.top, submitUserButtonRect.right - submitUserButtonRect.left, submitUserButtonRect.bottom - submitUserButtonRect.top, SWP_NOZORDER);
    }
    else {
        submitUserButton = CreateWindowW(
            L"BUTTON",                       // Predefined class; Unicode assumed
            L"Submit",                       // Button text
            WS_CHILD | WS_VISIBLE,           // Styles
            submitUserButtonRect.left,       // x position
            submitUserButtonRect.top,        // y position
            submitUserButtonRect.right - submitUserButtonRect.left,  // Button width
            submitUserButtonRect.bottom - submitUserButtonRect.top,  // Button height
            hWnd,                            // Parent window
            (HMENU)ID_BUTTON_SUBMIT_USER,         // No menu
            NULL,                            // Instance handle
            NULL                             // Additional application data
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
    DrawTextCenter(hdc, L"Add User", 5, width + 150, height);

    // Cleanup
    DeleteObject(hWelcomeFont);
    DeleteObject(hDarkBrush);
    DeleteObject(hSubsectionBrush);

    EndPaint(hWnd, &ps);
}

void AddUser::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height) {
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void AddUser::DestroyControls() {
    if (logoutButton != nullptr) {
        DestroyWindow(logoutButton);
        logoutButton = nullptr;
    }
    if (addUsernameLabel != nullptr) {
        DestroyWindow(addUsernameLabel);
        addUsernameLabel = nullptr;
    }
    if (addUsernameTextBox != nullptr) {
        DestroyWindow(addUsernameTextBox);
        addUsernameTextBox = nullptr;
    }
    if (addPasswordLabel != nullptr) {
        DestroyWindow(addPasswordLabel);
        addPasswordLabel = nullptr;
    }
    if (addPasswordTextBox != nullptr) {
        DestroyWindow(addPasswordTextBox);
        addPasswordTextBox = nullptr;
    }
    if (roleComboBox != nullptr) {
        DestroyWindow(roleComboBox);
        roleComboBox = nullptr;
    }
    if (addFirstNameLabel != nullptr) {
        DestroyWindow(addFirstNameLabel);
        addFirstNameLabel = nullptr;
    }
    if (addFirstNameTextBox != nullptr) {
        DestroyWindow(addFirstNameTextBox);
        addFirstNameTextBox = nullptr;
    }
    if (addLastNameLabel != nullptr) {
        DestroyWindow(addLastNameLabel);
        addLastNameLabel = nullptr;
    }
    if (addLastNameTextBox != nullptr) {
        DestroyWindow(addLastNameTextBox);
        addLastNameTextBox = nullptr;
    }
    if (addEmailLabel != nullptr) {
        DestroyWindow(addEmailLabel);
        addEmailLabel = nullptr;
    }
    if (addEmailTextBox != nullptr) {
        DestroyWindow(addEmailTextBox);
        addEmailTextBox = nullptr;
    }
    if (submitUserButton != nullptr) {
        DestroyWindow(submitUserButton);
        submitUserButton = nullptr;
    }
}