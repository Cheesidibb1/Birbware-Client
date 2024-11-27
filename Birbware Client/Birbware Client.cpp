#include <windows.h>
#include <string>
#include <gdiplus.h>   // GDI+ for loading PNG images
#include "resource.h"  // Include the resource header for identifiers
#pragma comment(lib, "gdiplus.lib")  // Link to the GDI+ library

using namespace Gdiplus;

// Window procedure function to handle events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hButton1, hButton2, hButton3;  // Buttons for three icons
    static ULONG_PTR gdiplusToken;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Initialize GDI+ library
        GdiplusStartupInput gdiPlusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiPlusStartupInput, NULL);

        // Create buttons for icons at the bottom
        hButton1 = CreateWindow(L"BUTTON", L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
            50, 600, 100, 100, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
        hButton2 = CreateWindow(L"BUTTON", L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
            200, 600, 100, 100, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
        hButton3 = CreateWindow(L"BUTTON", L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON,
            350, 600, 100, 100, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);

        // Load PNG images using GDI+ from resources
        HBITMAP hServerBitmap = NULL, hWrenchBitmap = NULL, hUserBitmap = NULL;

        // Load the PNG resources using GDI+
        Bitmap* pServerBitmap = Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SERVER));
        if (pServerBitmap && pServerBitmap->GetLastStatus() == Ok) {
            pServerBitmap->GetHBITMAP(Color(255, 255, 255), &hServerBitmap);
            SendMessage(hButton1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hServerBitmap);
        }

        Bitmap* pWrenchBitmap = Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_WRENCH));
        if (pWrenchBitmap && pWrenchBitmap->GetLastStatus() == Ok) {
            pWrenchBitmap->GetHBITMAP(Color(255, 255, 255), &hWrenchBitmap);
            SendMessage(hButton2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hWrenchBitmap);
        }

        Bitmap* pUserBitmap = Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_USER));
        if (pUserBitmap && pUserBitmap->GetLastStatus() == Ok) {
            pUserBitmap->GetHBITMAP(Color(255, 255, 255), &hUserBitmap);
            SendMessage(hButton3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hUserBitmap);
        }

        // Clean up
        delete pServerBitmap;
        delete pWrenchBitmap;
        delete pUserBitmap;
    }
    break;

    case WM_COMMAND:
        // Handle button click events
        if (LOWORD(wParam) == 1) {
            MessageBox(hwnd, L"Server Icon Button clicked", L"Info", MB_OK);
        }
        else if (LOWORD(wParam) == 2) {
            MessageBox(hwnd, L"Wrench Icon Button clicked", L"Info", MB_OK);
        }
        else if (LOWORD(wParam) == 3) {
            MessageBox(hwnd, L"User Icon Button clicked", L"Info", MB_OK);
        }
        break;

    case WM_DESTROY:
        // Shutdown GDI+ when window is destroyed
        GdiplusShutdown(gdiplusToken);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Correct WinMain signature
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Step 1: Initialize the window class
    const wchar_t* className = L"MyWindowClass";
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;  // Set WindowProc as the callback function
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Window class registration failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Step 2: Create the window with WS_OVERLAPPED style (no resizing)
    HWND hwnd = CreateWindowEx(
        0,
        className,
        L"My Application",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,  // No resize style here
        CW_USEDEFAULT, CW_USEDEFAULT, 950, 750,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Step 3: Load the main window icon from the resources
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  // Load the icon using the identifier defined in resource.h
    if (hIcon) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);  // Set the big icon
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);  // Set the small icon
    }
    else {
        MessageBox(hwnd, L"Failed to load icon", L"Error", MB_ICONERROR);  // Show error if loading fails
    }

    // Step 4: Show and update the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 5: Enter the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
