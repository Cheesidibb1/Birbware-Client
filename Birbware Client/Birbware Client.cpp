#include <windows.h>
#include <string>
#include "resource.h"  // Include the resource header for icon identifier

// Window procedure function to handle events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
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

    // Step 2: Create the window
    HWND hwnd = CreateWindowEx(
        0,
        className,
        L"My Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 650,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Step 3: Load the icon from the resources
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
