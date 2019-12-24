#ifndef GLCONTEXT_H
#define GLCONTEXT_H
#include <windows.h>
#include <sstream>
#include <string>
//
class GLContext
{
public:
    static GLContext& Instance()
    {
        static GLContext glc;
        return glc;
    }
    int InitHGLRC(HDC hDC, HGLRC* hGLRC, std::string version);
    //void InitScene(int Width, int Height);
    void DeInitHGLRC(HGLRC* hGLRC);
    void DetectOGLVersion();

private:
    GLContext() {}                                  
    ~GLContext() {}
    GLContext(const GLContext&);                 
    GLContext& operator=(const GLContext&);     
};
#endif
