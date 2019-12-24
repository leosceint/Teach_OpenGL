#include <windows.h>
#define GLEW_STATIC
#include "OpenGL.h"
#include "GLWindow.h"
#include "GLContext.h"

GLContext& GLRC = GLContext::Instance();
//
void init(int Width, int Height)
{
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

//
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 1.0f, 0.0f);  // Вверх
	glVertex3f(-1.0f, -1.0f, 0.0f);  // Слева снизу
	glVertex3f(1.0f, -1.0f, 0.0f);  // Справа снизу
	glEnd();
	glTranslatef(3.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);  // Слева вверху
	glVertex3f(1.0f, 1.0f, 0.0f);  // Справа вверху
	glVertex3f(1.0f, -1.0f, 0.0f);  // Справа внизу
	glVertex3f(-1.0f, -1.0f, 0.0f);  // Слева внизу
	glEnd();
}

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
		init(1000,500);
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
		if (wParam == VK_F1) GLRC.DetectOGLVersion();
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

	GLWindow WGL("HUD", hInstance, width, height, WindowCallback);
	WGL.Show();

	MSG msg;

	while (WGL.IsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				DrawGLScene();
				SwapBuffers(WGL.GetHDC());
			}
			else
			{
				WGL.IsRunning = false;
			}
		}
	}
	return 0;
}
