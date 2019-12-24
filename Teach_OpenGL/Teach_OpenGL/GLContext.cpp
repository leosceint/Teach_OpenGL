#include "OpenGL.h"
#include "GLContext.h"

int GLContext::InitHGLRC(HDC hDC, HGLRC* hGLRC, std::string version)
{
	int pos=version.find(".");
	std::istringstream iss(version.substr(0, pos) + ' ' + version.substr(pos + 1, version.length()),  std::istringstream::in);
	int major; int minor;
	iss >> major >> minor;

	PIXELFORMATDESCRIPTOR		pfd;
	HGLRC						hRCtemp;
	int							format;

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	int GL_attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, 0,
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

	GLenum error = glewInit();
	if (GLEW_OK != error)
	{
		MessageBox(NULL, "Glew Init - fail", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}
//
/*void InitScene(int Width, int Height) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);                      
	glDepthFunc(GL_LESS);                  
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);        
	glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();               
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}*/
//
void GLContext::DeInitHGLRC(HGLRC* hGLRC)
{
	ChangeDisplaySettings(NULL, 0);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(*hGLRC);
}
//
void GLContext::DetectOGLVersion()
{
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::ostringstream ss;
	ss << "\nVersion: " << major << "." << minor
		<< "\nRenderer: " << (const char*)glGetString(GL_RENDERER)
		<< "\nVendor: " << (const char*)glGetString(GL_VENDOR)
		<< "\nVersion2: " << (const char*)glGetString(GL_VERSION)
		<< "\nGLSL ver: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::string s = ss.str();
	MessageBox(NULL, s.c_str(), "Detected ver. Open GL", MB_OK);
}