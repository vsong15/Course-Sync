#include "Home.h"

void Home::Display(HWND hWnd)
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

    EndPaint(hWnd, &ps);
}

void Home::DrawTextCenter(HDC hdc, LPCWSTR text, int yPos, int width, int height)
{
    RECT rect;
    SetRect(&rect, 0, yPos, width, height);
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_TOP);
}
