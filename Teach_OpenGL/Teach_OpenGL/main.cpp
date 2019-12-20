#include <windows.h>
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/wglew.h"
#include <iostream>
#include <cstdlib>
#include <string>


HGLRC	m_hRC;
HDC		hDC;

void DetectOGLVersion() 
{
	/*int major, minor;
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
//
//
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
	
	PIXELFORMATDESCRIPTOR		pfd;
	HGLRC						hRCtemp;
	int							format;

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

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

	//Получаем дескриптор контекста окна
	hDC = GetDC(hWnd);
	if (!hDC) 
	{
		MessageBox(NULL, "Get DC - failed", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//Описание формата пикселей
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	format = ChoosePixelFormat(hDC, &pfd);
	if (!format || !SetPixelFormat(hDC, format, &pfd))
	{
		MessageBox(NULL, "Setting pixel format fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	hRCtemp = wglCreateContext(hDC);
	if (!hRCtemp || !wglMakeCurrent(hDC, hRCtemp))
	{
		MessageBox(NULL, "Сreating temp render context fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
		wglGetProcAddress("wglCreateContextAttribsARB"));

	if (!wglCreateContextAttribsARB) 
	{
		MessageBox(NULL, "wglCreateContextAttribsARB fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCtemp);

	m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if (!m_hRC || !wglMakeCurrent(hDC, m_hRC))
	{
		MessageBox(NULL, "Creating render context fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	//DetectOGLVersion();
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
