#include "GLContext.h"
float angle = 0.0f;

float points[] = {
   0.0f,  1.0f,  0.0f,
   1.0f, -1.0f,  0.0f,
  -1.0f, -1.0f,  0.0f
};

GLuint shader_programme;
GLuint vao;

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

	//�������� ������� ��������
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
		MessageBox(NULL, "�reating temp render context fail", "Error", MB_OK | MB_ICONERROR);
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
void GLContext::InitScene(int Width, int Height)
{
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	glShadeModel(GL_SMOOTH);        
	glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();               
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);	

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	const char* VertexShader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	const char* FragmentShader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &VertexShader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &FragmentShader, NULL);
	glCompileShader(fs);


	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
}
//
void GLContext::DrawScene() 
{
	if (1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glRotatef(angle, 0.0, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);  // �����
		glVertex2f(-1.0f, -1.0f);  // ����� �����
		glVertex2f(0.0f, -1.0f);  // ������ �����
		glEnd();
		glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5f, 1.0f);  // ����� ������
		glVertex2f(1.0f, 1.0f);  // ������ ������
		glVertex2f(1.0f, -1.0f);  // ������ �����
		glVertex2f(0.5f, -1.0f);  // ����� �����
		glEnd();
		angle += 0.2f;
	}
	// wipe the drawing surface clear
	if(0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
//
void GLContext::DeInitHGLRC(HGLRC* hGLRC)
{
	ChangeDisplaySettings(NULL, 0);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(*hGLRC);
}
//
void GLContext::DetectVersion()
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