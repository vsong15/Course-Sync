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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int activeWindow = 0;
    static bool gdiPlusInitialized = false;

    switch (message)
    {
    case WM_CREATE:
    {
        // Initialize GDI+ only once during the creation of the window
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&g_GdiplusToken, &gdiplusStartupInput, NULL);
        gdiPlusInitialized = true;
        return 0;
    }
    case WM_GETMINMAXINFO:
    {
        // Get the system's DPI scaling
        HDC hdc = GetDC(hWnd);
        int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
        int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
        ReleaseDC(hWnd, hdc);

        // Calculate the minimum window size based on the DPI scaling
        int minWidth = MulDiv(800, dpiX, 96); // Set the desired minimum width (800) and adjust based on DPI scaling
        int minHeight = MulDiv(600, dpiY, 96); // Set the desired minimum height (600) and adjust based on DPI scaling

        // Set the minimum window size
        MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
        pMinMaxInfo->ptMinTrackSize.x = minWidth;
        pMinMaxInfo->ptMinTrackSize.y = minHeight;

        return 0;
    }

    case WM_SIZE:
    {
        Login login;
        // Window size changed, reposition the error label
        if (login.errorLabel != nullptr)
        {
            int errorLabelWidth = 300; // Set the desired width of the error label
            int screenWidth = LOWORD(lParam); // Get the new window width

            // Calculate the left margin for centering the error label
            int leftMargin = (screenWidth - errorLabelWidth) / 2;

            // Update the error label position
            SetWindowPos(login.errorLabel, NULL, leftMargin, 150, errorLabelWidth, 25, SWP_NOZORDER);
        }
        return 0;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_BUTTON_LOGOUT:
            // Destroy controls in the active window
            if (activeWindow == 1) {
                Admin::DestroyControls();
            }

            // Clear the active window
            activeWindow = 0;          

            // Redraw the window content
            InvalidateRect(hWnd, NULL, TRUE);

            // Display the login window again
            Login login;
            login.Display(hWnd);

            // Show a message indicating successful logout
            MessageBox(hWnd, L"Logged out successfully.", L"Logout", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_BUTTON_LOGIN:
        {
            std::wstring username = Login::GetUsername();
            std::wstring password = Login::GetPassword();

            if (!username.empty() && !password.empty()) {
                if (DatabaseHelper::CheckUser(username.c_str(), password.c_str())) {
                    // Get the role of the user
                    std::string role = DatabaseHelper::GetRole(username.c_str(), password.c_str());
                    std::string firstName = DatabaseHelper::GetFirstName(username.c_str(), password.c_str());
                    std::string lastName = DatabaseHelper::GetLastName(username.c_str(), password.c_str());

                    if (role == "administrator") {
                        // Only display the home window if the role is "administrator"
                        Login login;
                        Login::DestroyControls();

                        Admin admin;
                        activeWindow = 1;
                        InvalidateRect(hWnd, NULL, TRUE);
                        admin.Display(hWnd);

                        // Construct the welcome message
                        std::wstring welcomeMessage = L"Welcome, " + std::wstring(firstName.begin(), firstName.end()) + L" " + std::wstring(lastName.begin(), lastName.end()) + L"!";

                        // Show the welcome message
                        MessageBox(hWnd, welcomeMessage.c_str(), L"Welcome", MB_OK | MB_ICONINFORMATION);
                    }
                    else {
                        // User is not an administrator, display an error message
                        Login::DisplayError(hWnd, L"Only administrators can log in.");
                    }
                }
                else {
                    // Incorrect username or password
                    Login::DisplayError(hWnd, L"Wrong username or password.");
                }
            }
            else {
                // Empty username or password field
                Login::DisplayError(hWnd, L"Please enter both username and password.");
            }
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
        if (activeWindow == 0) {
            Login::Display(hWnd);
        }
        else if (activeWindow == 1) {
            Admin::Display(hWnd);
        }
        break;
    case WM_DESTROY:
        if (gdiPlusInitialized)
        {
            // Shutdown GDI+ when the window is destroyed
            Gdiplus::GdiplusShutdown(g_GdiplusToken);
            gdiPlusInitialized = false;
        }

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
