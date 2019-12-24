#include <windows.h>
#include "OpenGL.h"
#include "GLWindow.h"
#include "GLContext.h"

GLContext& GLRC = GLContext::Instance();
int GlobalWidth = 0;
int GlobalHeight = 0;
//
//
LRESULT CALLBACK WindowCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC = NULL;
	HGLRC	m_hRC;

	switch (msg)
	{
	case WM_CREATE:
		//Получаем дескриптор контекста окна
		hDC = GetDC(hWnd);
		GLRC.InitHGLRC(hDC, &m_hRC, "3.1");
		GLRC.InitScene(GlobalWidth, GlobalHeight);
		break;
	case WM_CLOSE:
		GLRC.DeInitHGLRC(&m_hRC);
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_F1) GLRC.DetectVersion();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int CmdShow)
{
	int width = 1000;
	int height = 500;

	GlobalWidth = width;
	GlobalHeight = height;

	GLWindow WGL("HUD", hInstance, width, height, WindowCallback);
	WGL.Show();
	UpdateWindow(WGL.GetHWND());
	MSG msg;

	while (WGL.IsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				WGL.IsRunning = false;
			}
		}
		GLRC.DrawScene();
		SwapBuffers(WGL.GetHDC());
	}
	return 0;
}
