#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Global.h"

#include "R_OpenGL.h"
#include "R_Frame.h"

HDC hDC;
int OpenGLStarted=0;

HGLRC hRC;


int R_OpenGL_Initialize(HWND hWnd)
{
	int pixform;

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,							
		PFD_DRAW_TO_WINDOW |		
		PFD_SUPPORT_OPENGL | 
		PFD_DOUBLEBUFFER,			
		PFD_TYPE_RGBA,				
		32,						
		0, 0, 0, 0, 0, 0,			
		0,							
		0,							
		0,							
		0, 0, 0, 0,					
		16,							
		0,							
		0,							
		PFD_MAIN_PLANE,				
		0,							
		0, 0, 0						
	};

	
	DEVMODE dm;

	dm.dmSize=sizeof(DEVMODE);
	dm.dmFields=DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	dm.dmPelsWidth=ScreenX;
	dm.dmPelsHeight=ScreenY;
	dm.dmBitsPerPel=16;
	ChangeDisplaySettings(&dm,CDS_FULLSCREEN);

	hDC=GetDC(hWnd);
	pixform=ChoosePixelFormat(hDC,&pfd);
	if(!SetPixelFormat(hDC,pixform,&pfd)) 
	{
		MessageBox(NULL,"InitializeOpenGL(): Could not set pixel format","Info",MB_OK);
		return 0;
	}

	hRC=wglCreateContext(hDC);
	wglMakeCurrent(hDC,hRC);
	glViewport(0,0,ScreenX,ScreenY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV,(GLfloat)ScreenX/(GLfloat)ScreenY,0.03f,400.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.5f,0.5f,0.5f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);
	glDisable(GL_LIGHTING);

	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  
	/*glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE0+1);
	glEnable(GL_CLIP_PLANE0+2);
	glEnable(GL_CLIP_PLANE0+3);*/

	glPointSize(5.0);
	glLineWidth(1.0);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	
	OpenGLStarted=1;
	return 1;
}

void R_OpenGL_Shutdown(HWND hWnd)
{
	OpenGLStarted=0;
	ChangeDisplaySettings(NULL,0);
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd,hDC);
}

void R_OpenGL_ViewportSize(int x, int y)
{
/*	ScreenX=x;
	ScreenY=y;
	glViewport(0,0,ScreenX,ScreenY);*/
}