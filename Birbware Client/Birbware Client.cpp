#include <windows.h>

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
        // Display "Hello World" in the window
        TextOut(hdc, 50, 50, L"Hello World", 11); // Use L"..." for wide characters
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry point for a Windows GUI application (not `main`, but `WinMain`)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Step 1: Initialize the Window Class
    const wchar_t* className = L"SimpleWindowClass";  // Class name as a wide-character string (L"SimpleWindowClass")
    WNDCLASS wc = { 0 };  // Initialize all members to zero

    wc.lpfnWndProc = WindowProc;  // Set the window procedure
    wc.hInstance = hInstance;     // Use the provided hInstance
    wc.lpszClassName = className; // Assign the class name (wide-character string)

    if (!RegisterClass(&wc)) {  // Register the window class with Windows OS
        MessageBox(NULL, L"Window class registration failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Step 2: Create the Window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles
        className,                  // Window class name (wide-character string)
        L"Birbware Client",  // Window title (wide-character string)
        WS_OVERLAPPEDWINDOW,        // Window style (default style)
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 650,  // Size and position of the window
        NULL,                       // Parent window (NULL means no parent)
        NULL,                       // Menu (NULL means no menu)
        hInstance,                  // Instance handle
        NULL                        // Additional application data
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window creation failed!", L"Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);  // Show the window
    UpdateWindow(hwnd);          // Update the window

    // Step 3: Event loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);   // Translate keyboard messages
        DispatchMessage(&msg);    // Dispatch messages to the window procedure
    }

    return (int)msg.wParam;  // Return exit code
}
