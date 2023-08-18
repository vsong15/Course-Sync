// Course Sync.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Course Sync.h"
#include "Login.h"
#include "Admin.h"
#include "Constants.h"
#include "DatabaseHelper.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "AddUser.h"
#include "ViewUsers.h"
#include "UpdateUser.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
ULONG_PTR g_GdiplusToken = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COURSESYNC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSESYNC));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSESYNC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COURSESYNC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // Get the screen dimensions
   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);

   // Calculate the center position of the window
   int windowWidth = 800; // Set the desired width of the window
   int windowHeight = 600; // Set the desired height of the window
   int windowX = (screenWidth - windowWidth) / 2;
   int windowY = (screenHeight - windowHeight) / 2;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
       windowX, windowY, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
static int activeWindow = 0;
static bool gdiPlusInitialized = false;
void InitializeGdiPlus(HWND hWnd);
void SetMinimumWindowSize(HWND hWnd, LPARAM lParam);
void ShowLoginErrorLabel(HWND hWnd, LPARAM lParam);
void ButtonClicked(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void HandleVerticalScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
void ChangeActiveWindow(HWND hWnd);
void CleanUpGdiPlus();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        InitializeGdiPlus(hWnd);
        gdiPlusInitialized = true;
        return 0;
    case WM_GETMINMAXINFO:
        SetMinimumWindowSize(hWnd, lParam);
        DestroyWindow(ViewUsers::usersTable);
        ViewUsers::usersTable = nullptr;
        return 0;
    case WM_SIZE:
        ShowLoginErrorLabel(hWnd, lParam);
        return 0;
    case WM_COMMAND:
        ButtonClicked(hWnd, message, wParam, lParam);

        wchar_t selectedRoleName[100];
        if (LOWORD(wParam) == ID_COMBOBOX_ROLE && HIWORD(wParam) == CBN_SELCHANGE) {
            int selectedIndex = SendMessage(AddUser::roleComboBox, CB_GETCURSEL, 0, 0);
            switch (selectedIndex) {
            case 0:
                // Selected Administrator
                AddUser::setSelectedRole(1);
                break;
            case 1:
                // Selected Student
                AddUser::setSelectedRole(2);
                break;
            case 2:
                // Selected Faculty
                AddUser::setSelectedRole(3);
                break;
            case 3:
                // Selected Staff
                AddUser::setSelectedRole(4);
                break;
            }
        }
        return 0;
    case WM_VSCROLL:
        HandleVerticalScroll(hWnd, wParam, lParam);
        break;
    case WM_ERASEBKGND:
        return true;
    case WM_PAINT:
        ChangeActiveWindow(hWnd);
        break;
    case WM_DESTROY:
        CleanUpGdiPlus();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static Bitmap* pBitmapBuffer = nullptr; 
static Graphics* pBufferGraphics = nullptr;

void InitializeGdiPlus(HWND hWnd)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&g_GdiplusToken, &gdiplusStartupInput, NULL);

    pBitmapBuffer = new Bitmap(-1, -1);
    pBufferGraphics = new Graphics(pBitmapBuffer);
}

void SetMinimumWindowSize(HWND hWnd, LPARAM lParam) {
    HDC hdc = GetDC(hWnd);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(hWnd, hdc);

    int minWidth = MulDiv(800, dpiX, 96); 
    int minHeight = MulDiv(600, dpiY, 96); 

    MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
    pMinMaxInfo->ptMinTrackSize.x = minWidth;
    pMinMaxInfo->ptMinTrackSize.y = minHeight;
}

void ShowLoginErrorLabel(HWND hWnd, LPARAM lParam) {
    Login login;
    if (login.errorLabel != nullptr)
    {
        int errorLabelWidth = 300;
        int screenWidth = LOWORD(lParam);
        int leftMargin = (screenWidth - errorLabelWidth) / 2;
        SetWindowPos(login.errorLabel, NULL, leftMargin, 150, errorLabelWidth, 25, SWP_NOZORDER);
    }
}

void HandleDashboardButtonClick(HWND hWnd) {
    if (activeWindow == 2) {
        AddUser::DestroyControls();
    }
    else if (activeWindow == 3) {
        ViewUsers::DestroyControls();
    }
    else if (activeWindow == 4) {
        UpdateUser::DestroyControls();
    }
    activeWindow = 1;

    RECT contentRect;
    RECT rect;

    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    contentRect.left = 150;
    contentRect.top = 0;
    contentRect.right = width;
    contentRect.bottom = height;

    InvalidateRect(hWnd, &contentRect, TRUE);
}

void ButtonClicked(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId = LOWORD(wParam);
    switch (wmId)
    {
        case ID_BUTTON_LOGOUT:
        {
            if (activeWindow == 1) {
                Admin::DestroyControls();
            }
            else if (activeWindow == 2) {
                Admin::DestroyControls();
                AddUser::DestroyControls();
            }
            else if (activeWindow == 3) {
                Admin::DestroyControls();
                ViewUsers::DestroyControls();
            }
            else if (activeWindow == 4) {
                Admin::DestroyControls();
                UpdateUser::DestroyControls();
            }
            activeWindow = 0;
            InvalidateRect(hWnd, NULL, TRUE);
            Login login;
            login.Display(hWnd);
            MessageBox(hWnd, L"Logged out successfully.", L"Logout", MB_OK | MB_ICONINFORMATION);
            break;
        }
        case ID_BUTTON_LOGIN:
        {
            std::wstring username = Login::GetUsername();
            std::wstring password = Login::GetPassword();

            if (username.empty() || password.empty()) {
                Login::DisplayError(hWnd, L"Please enter both username and password.");
                break;
            }

            if (!DatabaseHelper::CheckUser(username.c_str(), password.c_str())) {
                Login::DisplayError(hWnd, L"Wrong username or password.");
                break;
            }

            int user_id = DatabaseHelper::GetUserID(username.c_str(), password.c_str());
            DatabaseHelper::StoreLoginTimestamp(user_id);
            std::string role = DatabaseHelper::GetRole(username.c_str(), password.c_str());

            if (role == "administrator") {
                std::string firstName = DatabaseHelper::GetFirstName(username.c_str(), password.c_str());
                std::string lastName = DatabaseHelper::GetLastName(username.c_str(), password.c_str());

                Admin::SetCurrentUserId(user_id);
                Login::DestroyControls();
                activeWindow = 1;
                InvalidateRect(hWnd, NULL, TRUE);

                Admin admin;
                admin.Display(hWnd);

                std::wstring welcomeMessage = L"Welcome, " + std::wstring(firstName.begin(), firstName.end()) + L" " + std::wstring(lastName.begin(), lastName.end()) + L"!";
                MessageBox(hWnd, welcomeMessage.c_str(), L"Welcome", MB_OK | MB_ICONINFORMATION);
            }
            else {
                Login::DisplayError(hWnd, L"Only administrators can log in.");
            }
            break;
        }
        case ID_BUTTON_DASHBOARD:
        {
            HandleDashboardButtonClick(hWnd);
            break;
        }
        case ID_BUTTON_ADD_USER:
        { 
            if (activeWindow == 1) {
                Admin::DestroyScrollBars();
                Admin::DestroyLogoutButton();
            }
            else if (activeWindow == 3) {
                ViewUsers::DestroyControls();
            }
            else if (activeWindow == 4) {
                UpdateUser::DestroyControls();
            }
            activeWindow = 2;
            RECT contentRect;

            RECT rect;
            GetClientRect(hWnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            contentRect.left = 150;
            contentRect.top = 0;
            contentRect.right = width;
            contentRect.bottom = height;
            InvalidateRect(hWnd, &contentRect, TRUE);
            break;
    
        }
        case ID_BUTTON_VIEW_USERS:
        {
            if (activeWindow == 1) {
                Admin::DestroyScrollBars();
                Admin::DestroyLogoutButton();
            }
            else if (activeWindow == 2) {
                AddUser::DestroyControls();
            }
            else if (activeWindow == 4) {
                UpdateUser::DestroyControls();
            }
            activeWindow = 3;
            RECT contentRect;

            RECT rect;
            GetClientRect(hWnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            contentRect.left = 150;
            contentRect.top = 0;
            contentRect.right = width;
            contentRect.bottom = height;
            InvalidateRect(hWnd, &contentRect, TRUE);
            break;
        }
        case ID_BUTTON_UPDATE_USER:
        {
            if (activeWindow == 1) {
                Admin::DestroyScrollBars();
                Admin::DestroyLogoutButton();
            }
            else if (activeWindow == 2) {
                AddUser::DestroyControls();
            }
            else if (activeWindow == 3) {
                ViewUsers::DestroyControls();
            }
            activeWindow = 4;
            RECT contentRect;

            RECT rect;
            GetClientRect(hWnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            contentRect.left = 150;
            contentRect.top = 0;
            contentRect.right = width;
            contentRect.bottom = height;
            InvalidateRect(hWnd, &contentRect, TRUE);
            break;
        }
        case ID_BUTTON_SUBMIT_USER:
        {
            int role = AddUser::selectedRole;
            std::wstring username = AddUser::GetUsername();
            std::wstring password = AddUser::GetPassword();
            std::wstring firstName = AddUser::GetFirstName();
            std::wstring lastName = AddUser::GetLastName();
            std::wstring email = AddUser::GetEmail();

            std::wstring roleName;
            switch (role) {
            case 1:
                roleName = L"Administrator";
                break;
            case 2:
                roleName = L"Student";
                break;
            case 3:
                roleName = L"Faculty";
                break;
            case 4:
                roleName = L"Staff";
                break;
            }

            // Insert the user information into the database
            if (DatabaseHelper::InsertUser(role, username, password, firstName, lastName, email)) {
                // Construct the message with the variable values
                std::wstring message = L"The following user has been created:"
                    L"\nRole: " + roleName +
                    L"\nUsername: " + username.c_str() +
                    L"\nPassword: " + password.c_str() +
                    L"\nFirst Name: " + firstName.c_str() +
                    L"\nLast Name: " + lastName.c_str() +
                    L"\nEmail: " + email.c_str();

                HandleDashboardButtonClick(hWnd);

                // Display an alert using MessageBox
                MessageBox(hWnd, message.c_str(), L"User Information", MB_OK | MB_ICONINFORMATION);
            }
            else {
                // Display an error alert if insertion failed
                MessageBox(hWnd, L"Failed to insert user information into the database.", L"Error", MB_OK | MB_ICONERROR);
            }
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
    }
}

void HandleVerticalScroll(HWND hWnd, WPARAM wParam, LPARAM lParam){
    int nScrollCode = LOWORD(wParam);
    int nPos = HIWORD(wParam);

    RECT updateRect;
    GetClientRect(hWnd, &updateRect);
    updateRect.top = 80;

    int width = updateRect.right - updateRect.left;
    int height = updateRect.bottom - updateRect.top;

    int minNavBarWidth = 150;
    int availableWidth = width - minNavBarWidth;
    int userManagementWidth = availableWidth * 2 / 3;

    if (lParam == (LPARAM)Admin::userManagementScrollBar) {
        EnableWindow(Admin::userManagementScrollBar, FALSE);
        int scrollAmount = 800; 
        switch (nScrollCode)
        {
        case SB_LINEUP:
            Admin::userManagementScrollPos -= scrollAmount;
            break;
        case SB_LINEDOWN:
            Admin::userManagementScrollPos += scrollAmount;
            break;
        case SB_THUMBPOSITION:
            Admin::loginActivityScrollPos = nPos;
            break;
        }
        RECT updateRect;
        GetClientRect(hWnd, &updateRect);
        updateRect.top = 80;

        RECT userManagementRect = { minNavBarWidth, 50, minNavBarWidth + userManagementWidth, height };
        updateRect.left = userManagementRect.left;
        updateRect.right = userManagementRect.right - 20;

        InvalidateRect(hWnd, &updateRect, FALSE);
    }
    else if (lParam == (LPARAM)Admin::loginActivityScrollBar) {
        EnableWindow(Admin::loginActivityScrollBar, FALSE);
        int scrollAmount = 800;

        switch (nScrollCode)
        {
        case SB_LINEUP:
            Admin::loginActivityScrollPos -= scrollAmount;
            break;
        case SB_LINEDOWN:
            Admin::loginActivityScrollPos += scrollAmount;
            break;
        case SB_THUMBPOSITION:
            Admin::loginActivityScrollPos = nPos;
            break;
        }

        RECT loginActivityRect = { minNavBarWidth + userManagementWidth, 50, width, height };
        updateRect.left = loginActivityRect.left;
        updateRect.right = loginActivityRect.right - 20;

        if (Admin::loginActivityScrollPos < 0) {
            Admin::loginActivityScrollPos = 0;
        }

        int maxScrollPos = Admin::totalContentHeight - (Admin::loginActivityRect.bottom - Admin::loginActivityRect.top);
        if (Admin::loginActivityScrollPos > maxScrollPos) {
            Admin::loginActivityScrollPos = maxScrollPos;
        }

        SCROLLINFO scrollInfo;
        scrollInfo.cbSize = sizeof(SCROLLINFO);
        scrollInfo.fMask = SIF_POS;
        scrollInfo.nPos = Admin::loginActivityScrollPos;

        SetScrollInfo(Admin::loginActivityScrollBar, SB_CTL, &scrollInfo, TRUE);
        InvalidateRect(hWnd, &updateRect, FALSE);
    }
}

void ChangeActiveWindow(HWND hWnd) {
    // Clear the buffer with a background color (optional)
    pBufferGraphics->Clear(Color(255, 255, 255, 255)); // Adjust the color as needed

    // Call the display function based on activeWindow value
    if (activeWindow == 0) {
        Login::Display(hWnd);
    }
    else if (activeWindow == 1) {
        Admin::Display(hWnd);
    }
    else if (activeWindow == 2) {
        AddUser::Display(hWnd);
    }
    else if (activeWindow == 3) {
        ViewUsers::Display(hWnd);
    }
    else if (activeWindow == 4) {
        UpdateUser::Display(hWnd);
    }

    // Draw onto the buffer using pBufferGraphics after displaying
    // For example:
    Pen pen(Color(255, 0, 0)); // Red pen
    pBufferGraphics->DrawLine(&pen, 200, 200, 400, 400); // Example drawing

    // Render the buffer onto the window
    Graphics windowGraphics(hWnd);
    windowGraphics.DrawImage(pBitmapBuffer, 0, 0);
}

void CleanUpGdiPlus()
{
    if (gdiPlusInitialized)
    {
        // Delete the offscreen buffer and its Graphics object
        delete pBufferGraphics;
        delete pBitmapBuffer;

        Gdiplus::GdiplusShutdown(g_GdiplusToken);
        gdiPlusInitialized = false;
    }
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;  
    }
    return (INT_PTR)FALSE;
}
