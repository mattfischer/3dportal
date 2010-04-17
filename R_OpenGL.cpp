#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "R_OpenGL.h"
#include "R_Frame.h"

namespace Render
{
    HDC hDC;
    bool started=false;

    HGLRC hRC;

    bool OpenGl::Initialize(HWND hWnd)
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
	    dm.dmPelsWidth=Frame::ScreenX;
	    dm.dmPelsHeight=Frame::ScreenY;
	    dm.dmBitsPerPel=16;
	    ChangeDisplaySettings(&dm,CDS_FULLSCREEN);

	    hDC=GetDC(hWnd);
	    pixform=ChoosePixelFormat(hDC,&pfd);
	    if(!SetPixelFormat(hDC,pixform,&pfd)) 
	    {
		    MessageBox(NULL,"InitializeOpenGL(): Could not set pixel format","Info",MB_OK);
		    return false;
	    }

	    hRC=wglCreateContext(hDC);
	    wglMakeCurrent(hDC,hRC);
	    glViewport(0,0,Frame::ScreenX,Frame::ScreenY);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(FOV,(GLfloat)Frame::ScreenX/(GLfloat)Frame::ScreenY,0.03f,400.f);
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
      
	    glPointSize(5.0);
	    glLineWidth(1.0);
    	
	    glCullFace(GL_BACK);
    	
    	
	    started=true;
	    return true;
    }

    void OpenGl::Shutdown(HWND hWnd)
    {
	    started=false;
	    ChangeDisplaySettings(NULL,0);
	    wglMakeCurrent(NULL,NULL);
	    wglDeleteContext(hRC);
	    ReleaseDC(hWnd,hDC);
    }

    bool OpenGl::Started()
    {
        return started;
    }

    void OpenGl::SwapBuffers()
    {
        ::SwapBuffers(hDC);
    }
}