#include "Login.h"
#include <Winuser.h>
#include "Constants.h"
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

HWND Login::usernameTextBox = nullptr;
HWND Login::passwordTextBox = nullptr;
HWND Login::loginButton = nullptr;
HWND Login::errorLabel = nullptr; // Define the static error label variable

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

    // Calculate the center position for the logo
    int logoWidth = 300; // Set the desired width of the logo
    int logoHeight = 60; // Set the desired height of the logo
    int logoLeftMargin = (width - logoWidth) / 2;
    int logoTopMargin = 10;

    // Load the image using GDI+
    Image image(L"courseSyncLogo.bmp");

    // Draw the logo
    Graphics graphics(hdc);
    graphics.DrawImage(&image, logoLeftMargin, logoTopMargin, logoWidth, logoHeight);

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

    // Define the desired font size for the username label
    int usernameLabelFontSize = 24;

    // Create the font with the desired size for the username label
    HFONT hUsernameLabelFont = CreateFont(
        usernameLabelFontSize, // Font height (size)
        0, // Width of characters (0 = default)
        0, // Angle of escapement (0 = default)
        0, // Orientation angle (0 = default)
        FW_NORMAL, // Font weight (normal)
        FALSE, // Italic
        FALSE, // Underline
        FALSE, // Strikeout
        DEFAULT_CHARSET, // Character set
        OUT_DEFAULT_PRECIS, // Output precision
        CLIP_DEFAULT_PRECIS, // Clipping precision
        DEFAULT_QUALITY, // Output quality
        DEFAULT_PITCH | FF_DONTCARE, // Pitch and family
        L"Arial" // Font face name
    );

    // Select the font into the device context for the username label
    HFONT hPrevUsernameLabelFont = (HFONT)SelectObject(hdc, hUsernameLabelFont);

    // Draw the username label with the updated font
    RECT usernameLabelRect;
    SetRect(&usernameLabelRect, newLeftMargin, 230, newLeftMargin + labelWidth, 260); // Updated position
    DrawText(hdc, L"Username:", -1, &usernameLabelRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOCLIP | DT_NOPREFIX);

    // Restore the previous font for the username label and release the new font
    SelectObject(hdc, hPrevUsernameLabelFont);
    DeleteObject(hUsernameLabelFont);

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

        HFONT hUsernameFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(usernameTextBox, WM_SETFONT, (WPARAM)hUsernameFont, TRUE);
    }

    // Define the desired font size for the password label
    int passwordLabelFontSize = 24;

    // Create the font with the desired size for the password label
    HFONT hPasswordLabelFont = CreateFont(
        passwordLabelFontSize, // Font height (size)
        0, // Width of characters (0 = default)
        0, // Angle of escapement (0 = default)
        0, // Orientation angle (0 = default)
        FW_NORMAL, // Font weight (normal)
        FALSE, // Italic
        FALSE, // Underline
        FALSE, // Strikeout
        DEFAULT_CHARSET, // Character set
        OUT_DEFAULT_PRECIS, // Output precision
        CLIP_DEFAULT_PRECIS, // Clipping precision
        DEFAULT_QUALITY, // Output quality
        DEFAULT_PITCH | FF_DONTCARE, // Pitch and family
        L"Arial" // Font face name
    );

    // Select the font into the device context for the password label
    HFONT hPrevPasswordLabelFont = (HFONT)SelectObject(hdc, hPasswordLabelFont);

    // Draw the password label with the updated font
    RECT passwordLabelRect;
    SetRect(&passwordLabelRect, newLeftMargin, 310, newLeftMargin + labelWidth, 340); // Updated position
    DrawText(hdc, L"Password:", -1, &passwordLabelRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOCLIP | DT_NOPREFIX);

    // Restore the previous font for the password label and release the new font
    SelectObject(hdc, hPrevPasswordLabelFont);
    DeleteObject(hPasswordLabelFont);

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

        HFONT hPasswordFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SendMessage(passwordTextBox, WM_SETFONT, (WPARAM)hPasswordFont, TRUE);
    }

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

    if (errorLabel != nullptr)
    {
        DestroyWindow(errorLabel);
        errorLabel = nullptr;
    }
}

std::wstring Login::GetUsername()
{
    int length = GetWindowTextLengthW(usernameTextBox);
    if (length > 0)
    {
        std::wstring buffer(length + 1, L'\0');
        GetWindowTextW(usernameTextBox, &buffer[0], length + 1);
        return buffer;
    }
    return L"";
}

std::wstring Login::GetPassword()
{
    int length = GetWindowTextLengthW(passwordTextBox);
    if (length > 0)
    {
        std::wstring buffer(length + 1, L'\0');
        GetWindowTextW(passwordTextBox, &buffer[0], length + 1);
        return buffer;
    }
    return L"";
}

void Login::DisplayError(HWND hWnd, LPCWSTR errorMessage)
{
    // Check if the error label already exists
    if (errorLabel != nullptr)
    {
        // If the error label already exists, update its text
        SetWindowTextW(errorLabel, errorMessage);
    }
    else
    {
        // If the error label doesn't exist, create it
        RECT clientRect;
        GetClientRect(hWnd, &clientRect); // Get the dimensions of the client area

        int errorLabelWidth = 300; // Set the desired width of the error label

        // Calculate the left margin for centering the error label within the client area
        int leftMargin = (clientRect.right - clientRect.left - errorLabelWidth) / 2;

        // Set the position above the text fields
        RECT errorRect = { leftMargin, 150, leftMargin + errorLabelWidth, 175 };

        errorLabel = CreateWindowW(
            L"STATIC",                                // Predefined class; Unicode assumed
            errorMessage,                              // Label text (error message)
            WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,  // Styles
            errorRect.left,                           // x position
            errorRect.top,                            // y position
            errorRect.right - errorRect.left,         // Label width
            errorRect.bottom - errorRect.top,         // Label height
            hWnd,                                     // Parent window
            NULL,                                     // No menu
            NULL,                                     // Instance handle
            NULL                                      // Additional application data
        );
    }
}
