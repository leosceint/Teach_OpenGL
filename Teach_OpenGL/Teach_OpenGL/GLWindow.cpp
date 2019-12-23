#include "GLWindow.h"

RECT GLWindow::GetDesktopResolution()
	{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	return desktop;
	}

GLWindow::GLWindow(LPCTSTR WindowName, HINSTANCE hInstance, int Width, int Height,
	LRESULT(WINAPI* pWndProc)(HWND, UINT, WPARAM, LPARAM))
{
	memset(&WindowClass, 0, sizeof(WindowClass));
	WindowClass.hInstance = 0;
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = pWndProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	WindowClass.lpszClassName = "GLWindow";

	if (!RegisterClassEx(&WindowClass))
	{
		MessageBox(NULL, "Register Class - failed", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	RECT Desktop = GetDesktopResolution();
	int X = static_cast<int>(0.5 * Desktop.right - 0.5*Width);
	int Y = static_cast<int>(0.5 * Desktop.bottom - 0.5*Height);

	DWORD style = WS_VISIBLE | WS_POPUP;
	DWORD exStyle = WS_EX_LAYERED;

	HWND hWnd = CreateWindowEx(exStyle, WindowClass.lpszClassName, WindowName, style,
		X, Y, Width, Height,
		NULL, NULL, 0, NULL);
	
	if (!hWnd) {
		MessageBox(NULL, "CreateWindowEx - failed", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	SetLayeredWindowAttributes(hWnd, 0x1, 0, LWA_COLORKEY);
	IsRunning = true;
	hDC = GetDC(hWnd);
}

HWND GLWindow::GetHWND() 
{
	return hWnd;
}

HDC GLWindow::GetHDC() 
{
	return hDC;
}

BOOL GLWindow::Show() 
{
	return ShowWindow(hWnd, SW_SHOW);
}