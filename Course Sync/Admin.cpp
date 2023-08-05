#include "Admin.h"
#include <Winuser.h>
#include "Constants.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

HWND Admin::logoutButton = nullptr;
HWND Admin::dashboardButton = nullptr;

void Admin::Display(HWND hWnd) {
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

    HBRUSH hDarkBrush = CreateSolidBrush(RGB(12, 42, 51)); 
    RECT navBarRect = { 0, 0, 150, height }; // Adjust the width as needed
    FillRect(hdc, &navBarRect, hDarkBrush);

    RECT dashboardButtonRect;
    int buttonWidth = 120; // Adjust the button width as needed
    int buttonHeight = 40; // Adjust the button height as needed
    int buttonX = 14;      // Adjust the X position as needed
    int buttonY = 60; // Adjust the Y position as needed

    SetRect(&dashboardButtonRect, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Update or create the login button
    if (dashboardButton != NULL)
    {
        SetWindowPos(dashboardButton, NULL, dashboardButtonRect.left, dashboardButtonRect.top, dashboardButtonRect.right - dashboardButtonRect.left, dashboardButtonRect.bottom - dashboardButtonRect.top, SWP_NOZORDER);
    }
    else
    {
        dashboardButton = CreateWindowW(
            L"BUTTON",                   // Predefined class; Unicode assumed
            L"Dashboard",                    // Button text
            WS_CHILD | WS_VISIBLE,       // Styles
            dashboardButtonRect.left,             // x position
            dashboardButtonRect.top,              // y position
            dashboardButtonRect.right - dashboardButtonRect.left,    // Button width
            dashboardButtonRect.bottom - dashboardButtonRect.top,    // Button height
            hWnd,                      // Parent window
            (HMENU)ID_BUTTON_DASHBOARD,                        // No menu
            NULL,                        // Instance handle
            NULL                         // Additional application data
        );
    }

    RECT logoutButtonRect;
    buttonWidth = 120; // Adjust the button width as needed
    buttonHeight = 40; // Adjust the button height as needed
    buttonX = 14;      // Adjust the X position as needed
    buttonY = height - buttonHeight - 10; // Adjust the Y position as needed

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

    // Draw the "Welcome to Course Sync" text
    HFONT hWelcomeFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hWelcomeFont);
    DrawTextCenter(hdc, L"Dashboard", 20, width, height);

    // Cleanup
    DeleteObject(hWelcomeFont);

    EndPaint(hWnd, &ps);
}

void Admin::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height) {
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void Admin::DestroyControls() {
    if (logoutButton != nullptr) {
        DestroyWindow(logoutButton);
        logoutButton = nullptr;
    }
    if (dashboardButton != nullptr) {
        DestroyWindow(dashboardButton);
        dashboardButton = nullptr;
    }
}