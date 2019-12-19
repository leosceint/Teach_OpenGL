#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>

void DetectOLGVersion() 
{
/*	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "\n\nOpenGL information:"
		<< "\n " << (const char*)glGetString(GL_RENDERER)
		<< "\n " << (const char*)glGetString(GL_VENDOR)
		<< "\n " << (const char*)glGetString(GL_VERSION)
		<< "\n " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< "\n " << major << "." << minor;
*/
}

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (msg) 
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	/*case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;*/
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WNDCLASSEX WindowClass;
	memset(&WindowClass, 0, sizeof(WindowClass));
	WindowClass.hInstance		= 0;
	WindowClass.cbSize			= sizeof(WNDCLASSEX);
	WindowClass.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.style			= CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc		= (WNDPROC)WindowCallback;
	WindowClass.cbClsExtra		= 0;
	WindowClass.cbWndExtra		= 0;
	WindowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WindowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	WindowClass.lpszClassName	= "Name";
	
	if (!RegisterClassEx(&WindowClass))
	{
		std::cout << "\nError 1";
		return -1;
	}

	int desktopWidth, desktopHeight;
	GetDesktopResolution(desktopWidth, desktopHeight);

	int width = 1000;
	int height = 500;

	DWORD	style, exStyle;
	style = WS_VISIBLE | WS_POPUP;
	exStyle = WS_EX_APPWINDOW;
	HWND hWnd = CreateWindowEx(exStyle, WindowClass.lpszClassName, "Window", style, desktopWidth / 2 - width / 2, desktopHeight / 2 - height / 2, width, height,
		NULL, NULL, 0, NULL);
	
	if (!hWnd) {
		MessageBox(NULL, "CreateWindowEx - failed", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);
	//UpdateWindow(hWnd);

	MSG msg;

	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else return 0;
		}
	}
	return 0;
}
