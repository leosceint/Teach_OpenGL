#include <windows.h>
#include "OpenGL.h"
#include "GLWindow.h"
#include <cstdlib>
#include <string>



void DetectOGLVersion() 
{
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::string s = std::to_string(major) + "." + std::to_string(minor);
	MessageBox(NULL, s.c_str(), "Detected ver. Open GL", MB_OK);
	/*std::cout << "\n\nOpenGL information:"
		<< "\n " << (const char*)glGetString(GL_RENDERER)
		<< "\n " << (const char*)glGetString(GL_VENDOR)
		<< "\n " << (const char*)glGetString(GL_VERSION)
		<< "\n " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< "\n " << major << "." << minor;
		*/
}
//
int InitHGLRC(HDC hDC, HGLRC* hGLRC) 
{
	PIXELFORMATDESCRIPTOR		pfd;
	HGLRC						hRCtemp;
	int							format;

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	int GL_attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

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

	*hGLRC = wglCreateContextAttribsARB(hDC, 0, GL_attribs);
	if (!(*hGLRC) || !wglMakeCurrent(hDC, *(hGLRC)))
	{
		MessageBox(NULL, "Creating render context fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}
//
void DeInitHGLRC(HDC hDC, HGLRC* hGLRC) 
{
	ChangeDisplaySettings(NULL, 0);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(*hGLRC);
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
		InitHGLRC(hDC, &m_hRC);
		break;
	case WM_CLOSE:
		DeInitHGLRC(hDC, &m_hRC);
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_F1) DetectOGLVersion();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//

//
GLvoid DrawGLScene(GLvoid) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

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
