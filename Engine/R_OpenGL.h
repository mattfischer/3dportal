#ifndef R_OPENGL_H
#define R_OPENGL_H

#include <windows.h>

int R_OpenGL_Initialize( HWND hWnd );
void R_OpenGL_Shutdown( HWND hWnd );
void R_OpenGL_ViewportSize( int x, int y );

extern int OpenGLStarted;
extern HDC hDC;

#endif