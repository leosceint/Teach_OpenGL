#ifndef GLWINDOW_H
#define GLWINDOW_H
#include <windows.h>

//=================================
//Класс окна
//=================================
class GLWindow 
{
public:
	GLWindow(LPCTSTR WindowName, HINSTANCE hInstance, int Width, int Height,
		LRESULT (WINAPI *pWndProc)(HWND, UINT, WPARAM, LPARAM));
	HWND GetHWND();
	HDC GetHDC();
	RECT GetDesktopResolution();
	BOOL Show();
public:
	bool IsRunning;
protected:
	HWND hWnd;
	HDC hDC;
	WNDCLASSEX WindowClass;
};
#endif