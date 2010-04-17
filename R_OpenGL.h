#ifndef R_OPENGL_H
#define R_OPENGL_H

#include <windows.h>

namespace Render
{
    class OpenGl
    {
    public:
        static bool Initialize( HWND hWnd );
        static void Shutdown( HWND hWnd );
        static bool Started();
        static void SwapBuffers();
    };
}

#endif