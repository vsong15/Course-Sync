#include "Admin.h"
#include <Winuser.h>
#include "Constants.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include "DatabaseHelper.h"

using namespace Gdiplus;

HWND Admin::logoutButton = nullptr;
HWND Admin::dashboardButton = nullptr;
HWND Admin::addUserButton = nullptr;
int Admin::currentUserId = 0;
int Admin::userManagementScrollPos = 0;
int Admin::loginActivityScrollPos = 0;
HWND Admin::userManagementScrollBar = nullptr;
HWND Admin::loginActivityScrollBar = nullptr;
int Admin::totalContentHeight = 0;
RECT Admin::loginActivityRect;

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

    RECT addUserButtonRect;
    buttonWidth = 120; // Adjust the button width as needed
    buttonHeight = 40; // Adjust the button height as needed
    buttonX = 14;      // Adjust the X position as needed
    buttonY = 120; // Adjust the Y position as needed

    SetRect(&addUserButtonRect, buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Update or create the login button
    if (addUserButton != NULL)
    {
        SetWindowPos(addUserButton, NULL, addUserButtonRect.left, addUserButtonRect.top, addUserButtonRect.right - addUserButtonRect.left, addUserButtonRect.bottom - addUserButtonRect.top, SWP_NOZORDER);
    }
    else
    {
        addUserButton = CreateWindowW(
            L"BUTTON",                   // Predefined class; Unicode assumed
            L"Add User",                    // Button text
            WS_CHILD | WS_VISIBLE,       // Styles
            addUserButtonRect.left,             // x position
            addUserButtonRect.top,              // y position
            addUserButtonRect.right - addUserButtonRect.left,    // Button width
            addUserButtonRect.bottom - addUserButtonRect.top,    // Button height
            hWnd,                      // Parent window
            (HMENU)ID_BUTTON_ADD_USER,                        // No menu
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

    HFONT hWelcomeFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hWelcomeFont);
    DrawTextCenter(hdc, L"Dashboard", 5, width + 150, height);

    // Calculate the section widths based on window width
    int minNavBarWidth = 150; // Minimum width for the navbar
    int availableWidth = width - minNavBarWidth;

    int userManagementWidth = availableWidth * 2 / 3;
    int loginActivityWidth = availableWidth - userManagementWidth;

    // Draw the section headings
    HFONT hSectionFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hSectionFont);

    // Draw rounded rectangles around the sections
    HBRUSH hSectionBrush = CreateSolidBrush(RGB(230, 230, 230)); // Color for the section background
    SelectObject(hdc, hSectionBrush);

    int cornerRadius = 10; // Adjust the corner radius as needed

    int textTopMargin = 5; // Adjust the top margin for text as needed

    // User Management section
    RECT userManagementRect = { minNavBarWidth, 50, minNavBarWidth + userManagementWidth, height };
    RoundRect(hdc, userManagementRect.left, userManagementRect.top, userManagementRect.right, userManagementRect.bottom, cornerRadius, cornerRadius);

    // Login Activity section
    loginActivityRect = { minNavBarWidth + userManagementWidth, 50, width, height };
    RoundRect(hdc, loginActivityRect.left, loginActivityRect.top, loginActivityRect.right, loginActivityRect.bottom, cornerRadius, cornerRadius);

    // Check if the section titles would overlap with the navbar
    if (userManagementRect.left < minNavBarWidth) {
        OffsetRect(&userManagementRect, minNavBarWidth - userManagementRect.left, 0);
        OffsetRect(&loginActivityRect, minNavBarWidth - userManagementRect.left, 0);
    }

    // Adjust the top coordinate of the RECT structures for text positioning
    userManagementRect.top += textTopMargin;
    loginActivityRect.top += textTopMargin;

    // Set the background color for the DrawText function to match the section's background
    SetBkColor(hdc, RGB(230, 230, 230)); // Light gray section background

    DrawText(hdc, L"User Management Activity", -1, &userManagementRect, DT_SINGLELINE | DT_CENTER | DT_TOP);
    DrawText(hdc, L"Login Activity", -1, &loginActivityRect, DT_SINGLELINE | DT_CENTER | DT_TOP);

    if (userManagementScrollBar != NULL) {
        SetWindowPos(userManagementScrollBar, NULL, userManagementRect.right - 20, userManagementRect.top, 20, userManagementRect.bottom - userManagementRect.top, SWP_NOZORDER);
    }
    else {
        // Add vertical scroll bars to manage scrolling for the sections
        userManagementScrollBar = CreateWindowW(L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT,
            userManagementRect.right - 20, userManagementRect.top, 20, userManagementRect.bottom - userManagementRect.top, hWnd, NULL, NULL, NULL);
    }
   
    if (loginActivityScrollBar != NULL) {
        SetWindowPos(loginActivityScrollBar, NULL, loginActivityRect.right - 20, loginActivityRect.top, 20, loginActivityRect.bottom - loginActivityRect.top, SWP_NOZORDER);
    }
    else {
        loginActivityScrollBar = CreateWindowW(L"SCROLLBAR", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT,
            loginActivityRect.right - 20, loginActivityRect.top, 20, loginActivityRect.bottom - loginActivityRect.top, hWnd, NULL, NULL, NULL);
    }

    // Adjust font size based on screen width
    int fontSizeDivisor = 80; // Adjust the divisor as needed
    int fontSize = width / fontSizeDivisor;
    HFONT hTimestampFont = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hTimestampFont);

    // Retrieve login timestamps using the DatabaseHelper function
    int user_id = Admin::GetCurrentUserId();
    std::vector<std::string> login_timestamps = DatabaseHelper::GetLoginTimestamps(user_id);

    // Display the login timestamps
    int startY = loginActivityRect.top + textTopMargin + 40 - loginActivityScrollPos; // Adjusted startY
    int lineHeight = fontSize;

    // Store the initial startY value
    int initialStartY = startY;

    // Calculate the total height of the content within the section
    totalContentHeight = login_timestamps.size() * lineHeight;

    HBRUSH hSubsectionBrush = CreateSolidBrush(RGB(230, 230, 230)); // Color for the subsection background
    SelectObject(hdc, hSubsectionBrush);

    // Create a RECT for login timestamps
    RECT loginTimestampsRect = loginActivityRect;
    loginTimestampsRect.top = startY;
    loginTimestampsRect.left += 5;

    // Array to map month numbers to their names
    const wchar_t* monthNames[] = {
        L"January", L"February", L"March", L"April", L"May", L"June",
        L"July", L"August", L"September", L"October", L"November", L"December"
    };

    // Retrieve the user's full name outside the loop
    std::wstring fullName = DatabaseHelper::GetFullNameFromUserID(user_id);

    // Calculate the Y position of the "Login Activity" section title
    int loginActivityTitleY = loginActivityRect.top + textTopMargin + 20;

    for (const std::string& timestamp : login_timestamps) {
        if (loginTimestampsRect.top + lineHeight > height - textTopMargin) {
            break;
        }

        // Check if the timestamp is too high and overlaps with the title
        if (loginTimestampsRect.top < loginActivityTitleY) {
            // Skip displaying this timestamp as it's too high
            loginTimestampsRect.top += lineHeight; // Move to the next line
            continue;
        }

        std::wstring wtimestamp;
        wtimestamp.assign(timestamp.begin(), timestamp.end());

        // Extract year, month, day, hour, minute, and seconds from the timestamp string
        int year, month, day, hour, minute, second;
        swscanf_s(wtimestamp.c_str(), L"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

        // Get the month name from the array
        const wchar_t* monthName = monthNames[month - 1];

        // Format the timestamp along with the user's full name
        wchar_t formattedTimestamp[150];
        const wchar_t* amPm = (hour < 12) ? L"AM" : L"PM";
        hour = (hour % 12 == 0) ? 12 : hour % 12;
        swprintf_s(formattedTimestamp, L"[%s %d, %d - %02d:%02d %s] User: %ls", monthName, day, year, hour, minute, amPm, fullName.c_str());

        // Check if the timestamp should be displayed below the "Login Activity" section title
        if (loginTimestampsRect.top >= loginActivityRect.top + textTopMargin + 10) {
            // Draw the formatted timestamp
            SetTextColor(hdc, RGB(53, 99, 158));
            DrawTextW(hdc, formattedTimestamp, -1, &loginTimestampsRect, DT_SINGLELINE | DT_LEFT | DT_CENTER | DT_TOP);
        }

        // Restore the initial startY value for consistent positioning
        startY = initialStartY;

        startY += lineHeight;
        loginTimestampsRect.top += lineHeight; // Move to the next line

        // Calculate the bottom position of the last displayed timestamp
        int lastTimestampBottom = startY + totalContentHeight - lineHeight;

        // Check if the bottom position of the last timestamp is within the visible section
        if (lastTimestampBottom < loginActivityRect.bottom - textTopMargin) {
            // Adjust the scroll position so that the last timestamp is visible
            Admin::loginActivityScrollPos = totalContentHeight - (loginActivityRect.bottom - textTopMargin - startY);
        }
    }

    // Customize the scrollbar appearance
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_ALL;
    scrollInfo.nMin = 0;
    scrollInfo.nMax = totalContentHeight; // Set the total content height
    scrollInfo.nPage = height; // Set the height of the visible area
    scrollInfo.nPos = Admin::loginActivityScrollPos;

    // Make sure that the current scroll position is within the valid range
    if (Admin::loginActivityScrollPos > scrollInfo.nMax) {
        Admin::loginActivityScrollPos = scrollInfo.nMax;
    }

    SetScrollInfo(loginActivityScrollBar, SB_CTL, &scrollInfo, TRUE);

    // Cleanup
    DeleteObject(hWelcomeFont);
    DeleteObject(hDarkBrush);
    DeleteObject(hSectionFont);
    DeleteObject(hSectionBrush);
    DeleteObject(hTimestampFont);
    DeleteObject(hSubsectionBrush);

    EndPaint(hWnd, &ps);
}

void Admin::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height) {
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void Admin::DestroyControls() {
    // Reset scroll positions
    userManagementScrollPos = 0;
    loginActivityScrollPos = 0;

    if (logoutButton != nullptr) {
        DestroyWindow(logoutButton);
        logoutButton = nullptr;
    }
    if (dashboardButton != nullptr) {
        DestroyWindow(dashboardButton);
        dashboardButton = nullptr;
    }
    if (addUserButton != nullptr) {
        DestroyWindow(addUserButton);
        addUserButton = nullptr;
    }
}
