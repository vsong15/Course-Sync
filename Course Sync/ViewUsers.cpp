#include "ViewUsers.h"
#include <Winuser.h>
#include "Constants.h"
#include <CommCtrl.h>
#include <gdiplus.h>
#include "DatabaseHelper.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

HWND ViewUsers::logoutButton = nullptr;
HWND ViewUsers::usersTable = nullptr;

void ViewUsers::Display(HWND hWnd) {
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


    RECT viewUsersTableRect;
    int viewUsersTableWidth = sectionWidth * 0.9; // Change variable name to submitUserButtonWidth
    int viewUsersTableHeight = sectionHeight * 0.8; // Change variable name to submitUserButtonHeight
    int viewUsersTableX = (subsectionRect.left + subsectionRect.right - viewUsersTableWidth) / 2;
    int viewUsersTableY = (sectionHeight - viewUsersTableHeight + 50) / 2;

    SetRect(&viewUsersTableRect, viewUsersTableX, viewUsersTableY, viewUsersTableX + viewUsersTableWidth, viewUsersTableY + viewUsersTableHeight);

    if (usersTable != NULL) {
        SetWindowPos(usersTable, NULL, viewUsersTableRect.left, viewUsersTableRect.top, viewUsersTableRect.right - viewUsersTableRect.left, viewUsersTableRect.bottom - viewUsersTableRect.top, SWP_NOZORDER);
    }
    else {
        // Create the table control
        usersTable = CreateWindow(
            WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER,
            viewUsersTableRect.left,       // x position
            viewUsersTableRect.top,        // y position
            viewUsersTableRect.right - viewUsersTableRect.left,  // Button width
            viewUsersTableRect.bottom - viewUsersTableRect.top,  // Button height
            hWnd, 
            NULL, 
            NULL, 
            NULL)
        ;
        PopulateTable(hWnd, viewUsersTableWidth, viewUsersTableHeight);
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
    DrawTextCenter(hdc, L"View Users", 5, width + 150, height);

    // Cleanup
    DeleteObject(hWelcomeFont);
    DeleteObject(hDarkBrush);
    DeleteObject(hSubsectionBrush);

    EndPaint(hWnd, &ps);
}

void ViewUsers::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height) {
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}

void ViewUsers::DestroyControls() {
    if (logoutButton != nullptr) {
        DestroyWindow(logoutButton);
        logoutButton = nullptr;
    }
    if (usersTable != nullptr) {
        DestroyWindow(usersTable);
        usersTable = nullptr;
    }
}

void ViewUsers::PopulateTable(HWND hWnd, int sectionWidth, int sectionHeight) {
    // Casted LPWSTR strings
    LPWSTR userIdColumn = const_cast<LPWSTR>(L"User ID");
    LPWSTR usernameColumn = const_cast<LPWSTR>(L"Username");
    LPWSTR roleColumn = const_cast<LPWSTR>(L"Role");
    LPWSTR firstNameColumn = const_cast<LPWSTR>(L"First Name");
    LPWSTR lastNameColumn = const_cast<LPWSTR>(L"Last Name");
    LPWSTR emailColumn = const_cast<LPWSTR>(L"Email");

    // Add columns to the table
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    lvColumn.fmt = LVCFMT_LEFT;
    lvColumn.cx = sectionWidth / 6; // You can adjust the column width as needed

    lvColumn.pszText = userIdColumn;
    ListView_InsertColumn(usersTable, 0, &lvColumn);

    lvColumn.pszText = usernameColumn;
    ListView_InsertColumn(usersTable, 1, &lvColumn);

    lvColumn.pszText = roleColumn;
    ListView_InsertColumn(usersTable, 2, &lvColumn);

    lvColumn.pszText = firstNameColumn;
    ListView_InsertColumn(usersTable, 3, &lvColumn);

    lvColumn.pszText = lastNameColumn;
    ListView_InsertColumn(usersTable, 4, &lvColumn);

    lvColumn.pszText = emailColumn;
    ListView_InsertColumn(usersTable, 5, &lvColumn);

    // Populate the table with data from the database
    DatabaseHelper::PopulateTableFromDatabase(usersTable);
}