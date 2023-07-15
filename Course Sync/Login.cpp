#include "Login.h"
#include <Winuser.h>
#include "Constants.h"

HWND Login::usernameTextBox = nullptr;
HWND Login::passwordTextBox = nullptr;
HWND Login::loginButton = nullptr;

void Login::Display(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // Set the background color
    HBRUSH hBrush = CreateSolidBrush(RGB(12, 44, 52)); // #0c2c34
    FillRect(hdc, &ps.rcPaint, hBrush);

    // Set the text color
    SetTextColor(hdc, RGB(177, 207, 241));    // #365a81

    // Set the text background color to match the main window background color
    SetBkColor(hdc, RGB(12, 44, 52));       // #0c2c34

    // Get the client area dimensions
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Draw the title
    HFONT hTitleFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hTitleFont);
    DrawTextCenter(hdc, L"Course Sync", 20, width, height);

    // Draw the description
    HFONT hDescFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hDescFont);
    DrawTextCenter(hdc, L"Student Enrollment Database Management System", 80, width, height);

    // Calculate the label and text field widths
    int labelWidth = 150;
    int textFieldWidth = 200;

    // Calculate the left margin for centering
    int leftMargin = (width - (labelWidth + textFieldWidth)) / 2;

    // Adjust the left and right margins
    int newLeftMargin = leftMargin - 40;  // Decrease the left margin
    int newRightMargin = leftMargin + textFieldWidth + labelWidth + 40;  // Increase the right margin

    // Draw the username label
    HFONT hLabelFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hLabelFont);
    RECT usernameLabelRect;
    SetRect(&usernameLabelRect, newLeftMargin, 230, newLeftMargin + labelWidth, 260); // Updated position
    DrawText(hdc, L"Username:", -1, &usernameLabelRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOCLIP | DT_NOPREFIX);

    // Update or create the username input field
    RECT usernameRect;
    SetRect(&usernameRect, newLeftMargin + labelWidth, 230, newRightMargin, 260); // Updated position
    if (usernameTextBox != NULL)
    {
        SetWindowPos(usernameTextBox, NULL, usernameRect.left, usernameRect.top, usernameRect.right - usernameRect.left, usernameRect.bottom - usernameRect.top, SWP_NOZORDER);
    }
    else
    {
        usernameTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            usernameRect.left, usernameRect.top, usernameRect.right - usernameRect.left, usernameRect.bottom - usernameRect.top,
            hWnd, NULL, NULL, NULL);
    }

    // Draw the password label
    RECT passwordLabelRect;
    SetRect(&passwordLabelRect, newLeftMargin, 310, newLeftMargin + labelWidth, 340); // Updated position
    DrawText(hdc, L"Password:", -1, &passwordLabelRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOCLIP | DT_NOPREFIX);

    // Update or create the password input field
    RECT passwordRect;
    SetRect(&passwordRect, newLeftMargin + labelWidth, 310, newRightMargin, 340); // Updated position
    if (passwordTextBox != NULL)
    {
        SetWindowPos(passwordTextBox, NULL, passwordRect.left, passwordRect.top, passwordRect.right - passwordRect.left, passwordRect.bottom - passwordRect.top, SWP_NOZORDER);
    }
    else
    {
        passwordTextBox = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD,
            passwordRect.left, passwordRect.top, passwordRect.right - passwordRect.left, passwordRect.bottom - passwordRect.top,
            hWnd, NULL, NULL, NULL);
    }

    // Cleanup
    DeleteObject(hLabelFont);

    // Draw the login button
    RECT buttonRect;
    SetRect(&buttonRect, width / 2 - 80, 380, width / 2 + 80, 420);

    // Update or create the login button
    if (loginButton != NULL)
    {
        SetWindowPos(loginButton, NULL, buttonRect.left, buttonRect.top, buttonRect.right - buttonRect.left, buttonRect.bottom - buttonRect.top, SWP_NOZORDER);
    }
    else
    {
        loginButton = CreateWindowW(
            L"BUTTON",                   // Predefined class; Unicode assumed
            L"Login",                    // Button text
            WS_CHILD | WS_VISIBLE,       // Styles
            buttonRect.left,             // x position
            buttonRect.top,              // y position
            buttonRect.right - buttonRect.left,    // Button width
            buttonRect.bottom - buttonRect.top,    // Button height
            hWnd,                      // Parent window
            (HMENU)ID_BUTTON_LOGIN,                        // No menu
            NULL,                        // Instance handle
            NULL                         // Additional application data
        );
    }

    // Set the background mode to transparent
    SetBkMode(hdc, TRANSPARENT);

    // Calculate the center position
    int textHeight = buttonRect.bottom - buttonRect.top;
    SIZE textSize;
    GetTextExtentPoint32(hdc, L"Login", 5, &textSize); // Measure the text size

}

void Login::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height)
{
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void Login::DestroyControls()
{
    if (usernameTextBox)
    {
        DestroyWindow(usernameTextBox);
        usernameTextBox = NULL;
    }

    if (passwordTextBox)
    {
        DestroyWindow(passwordTextBox);
        passwordTextBox = NULL;
    }

    if (loginButton)
    {
        DestroyWindow(loginButton);
        loginButton = NULL;
    }
}