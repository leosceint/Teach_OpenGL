#include "GL/glew.h"
#include "GL/glut.h"
#include <iostream>

void DetectOLGVersion() 
{
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "\n\nOpenGL information:"
		<< "\n " << (const char*)glGetString(GL_RENDERER)
		<< "\n " << (const char*)glGetString(GL_VENDOR)
		<< "\n " << (const char*)glGetString(GL_VERSION)
		<< "\n " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< "\n " << major << "." << minor;
}

void RenderScene() 
{
	DetectOLGVersion();
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Texxt");
	glutDisplayFunc(RenderScene);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glutMainLoop();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	return 0;
}