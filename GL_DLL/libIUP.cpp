// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "libIUP.h"
#include <string>
#include <iostream>

#ifdef  assert
#define verify(expr) if(!expr) assert(0)
#else verify(expr) expr
#endif

const TCHAR szAppName[] = _T("TransparentGL");
const TCHAR wcWndName[] = _T("WS_EX_LAYERED OpenGL");

HDC hDC;
HGLRC m_hrc;
int w;
int h;

HDC pdcDIB;
HBITMAP hbmpDIB;
void* bmp_cnt(NULL);
int cxDIB(0);
int cyDIB(0);
BITMAPINFOHEADER BIH;

GLvoid CreateShaderProgram();
GLvoid LoadTex(std::string path);

BOOL initSC()
{
	LoadTex("C:\\Projects\\AeroGLWinAPI\\GL_DLL\\glass.png");

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	return 0;
}

void resizeSC(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

BOOL renderSC()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
	glColor3f(1.0f, 0.0f, 0.0f);                      // Set The Color To Red
	glVertex3f(0.0f, 1.0f, 0.0f);                  // Top
	glColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
	glVertex3f(-1.0f, -1.0f, 0.0f);                  // Bottom Left
	glColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
	glVertex3f(1.0f, -1.0f, 0.0f);                  // Bottom Right
	glEnd();

	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
	glColor3f(1.0f, 1.0f, 1.0f);                      // Set The Color To Red
	glVertex3f(0.0f, -1.0f, 0.0f);                  // Top
	glColor3f(1.0f, 1.0f, 1.0f);                      // Set The Color To Green
	glVertex3f(1.0f, 1.0f, 0.0f);                  // Bottom Left
	glColor3f(1.0f, 1.0f, 1.0f);                      // Set The Color To Blue
	glVertex3f(-1.0f, 1.0f, 0.0f);                  // Bottom Right
	glEnd();

	glPopMatrix();
	glFlush();
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "\n\nOpenGL information:"
		<<"\n "<< (const char*)glGetString(GL_RENDERER)
		<<"\n " << (const char*)glGetString(GL_VENDOR)
		<<"\n " << (const char*)glGetString(GL_VERSION)
		//<<"\n " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
		<<"\n " << major<<"." << minor;
	//CreateShaderProgram();

	return 0;
}

// DIB -> hDC
void draw(HDC pdcDest)
{
	assert(pdcDIB);

	verify(BitBlt(pdcDest, 0, 0, w, h, pdcDIB, 0, 0, SRCCOPY));
}

void CreateDIB(int cx, int cy)
{
	assert(cx > 0);
	assert(cy > 0);

	cxDIB = cx;
	cyDIB = cy;

	int iSize = sizeof(BITMAPINFOHEADER);
	memset(&BIH, 0, iSize);

	BIH.biSize = iSize;
	BIH.biWidth = cx;
	BIH.biHeight = cy;
	BIH.biPlanes = 1;
	BIH.biBitCount = 24;
	BIH.biCompression = BI_RGB;

	if (pdcDIB)
		verify(DeleteDC(pdcDIB));

	pdcDIB = CreateCompatibleDC(NULL);
	assert(pdcDIB);

	if (hbmpDIB)
		verify(DeleteObject(hbmpDIB));

	hbmpDIB = CreateDIBSection(
		pdcDIB,
		(BITMAPINFO*)&BIH,
		DIB_RGB_COLORS,
		&bmp_cnt,
		NULL,
		0);

	assert(hbmpDIB);
	assert(bmp_cnt);

	if (hbmpDIB)
		SelectObject(pdcDIB, hbmpDIB);
}

BOOL CreateHGLRC()
{
	DWORD dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = dwFlags;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int PixelFormat = ChoosePixelFormat(pdcDIB, &pfd);
	if (PixelFormat == 0) {
		assert(0);
		return FALSE;
	}

	BOOL bResult = SetPixelFormat(pdcDIB, PixelFormat, &pfd);
	if (bResult == FALSE) {
		assert(0);
		return FALSE;
	}

	m_hrc = wglCreateContext(pdcDIB);
	if (!m_hrc) {
		assert(0);
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return 0;
		break;

	case WM_CREATE:
		break;

	case WM_DESTROY:
		if (m_hrc)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hrc);
		}
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		renderSC(); // OpenGL -> DIB
		draw(hDC);  // DIB -> hDC
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		w = LOWORD(lParam); h = HIWORD(lParam);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);

		CreateDIB(w, h);
		CreateHGLRC();
		verify(wglMakeCurrent(pdcDIB, m_hrc));

		initSC();
		resizeSC(w, h);
		renderSC();
		break;

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

LIBIUP_API int LIBIUP_Main(int width, int height)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = szAppName;

	if (!RegisterClassEx(&wc))
	{
		return FALSE;
	}

	int desktopWidth, desktopHeight;
	GetDesktopResolution(desktopWidth, desktopHeight);

	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szAppName, wcWndName,
		WS_VISIBLE | WS_POPUP, desktopWidth / 2 - width / 2, desktopHeight / 2 - height / 2, width, height,
		NULL, NULL, 0, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	verify(SetLayeredWindowAttributes(hWnd, 0x0, 0, LWA_COLORKEY));

	MSG msg;
	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else return 0;
		}
	}
	return TRUE;
}

GLvoid LoadImage(std::string path)
{
	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	static unsigned char* emptyImage = 0;
	if (emptyImage == 0)
	{
		emptyImage = (unsigned char*)calloc(width * height * 3, sizeof(unsigned char));
		memset(emptyImage, 255, width * height * 3);
	}
	if (image == nullptr)
	{
		//cout << "Image does not exist" << endl;//Оповестить, если нет изображения.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyImage);
	}
	else
	{
		//Загружаем изображение в текстуру
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		SOIL_free_image_data(image);//освобождаем память
	}
}

GLvoid LoadTex(std::string path)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LoadImage(path);
}

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * vec4(position, 1.0f);\n"
"TexCoord = texCoord;\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D texture;\n"
"void main()\n"
"{\n"
"color = texture(texture, TexCoord);\n"
"}\0";

GLuint LinkShaders()//функция создания и сборки шейдеров
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLvoid CreateShaderProgram()
{
	glewExperimental = GL_TRUE;

	GLfloat vertices[] = {
		// Positions        // Texture Coords
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top Right		0-2
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right	1-3
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left	2-0	
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left		3-1
	};
	GLuint indexs[] = {
		0, 1, 3,
		1, 2, 3
	};
	GLuint VBO=0, VAO=0, IBO=0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//передаем точки в VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	//создание и сборка шейдерной программы
	GLuint shaderProgram = LinkShaders();
}