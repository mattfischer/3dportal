#ifndef R_OPENGL_H
#define R_OPENGL_H

#include <windows.h>

namespace Render
{
    int OpenGL_Initialize( HWND hWnd );
    void OpenGL_Shutdown( HWND hWnd );
    void OpenGL_ViewportSize( int x, int y );

    extern int OpenGLStarted;
    extern HDC hDC;
}

#endif