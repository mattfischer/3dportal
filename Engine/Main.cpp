#include <windows.h>

#include "Global.h"

#include "R_OpenGL.h"
#include "R_Frame.h"

#include "G_Game.h"

#include "I_Input.h"

#include "G_Console.h"

#include "S_Manager.h"

HWND hWnd;
HWND hFPSWindow;

void QuitGame()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		/*
	case WM_KEYDOWN:
		if(wParam==VK_ESCAPE)
			I_EnableInput(false);
		return 0;
	case WM_LBUTTONDOWN:
		I_EnableInput(true);
		return 0;
	case WM_SIZE:
		R_OpenGL_ViewportSize(LOWORD(lParam), HIWORD(lParam));
		return 0;*/
	case WM_DESTROY:
		Render::OpenGL_Shutdown(hWnd);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}

char GOBPath[100];
char levelName[100];

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,LPSTR Cmdline,int nShow)
{
	MSG msg;
	WNDCLASSEX wc;
	int i,j;

	i=0;
	j=i;
	while(Cmdline[j]!=' ' && Cmdline[j]!='\0') j++;
		
	if(j==i) strcpy(levelName, "09fuelstation.jkl");
	else
	{
		strncpy(levelName, Cmdline, j);
		levelName[j]='\0';
	}

	if(Cmdline[j]!='\0')
	{
		j++;
		i=j;
		while(Cmdline[j]!=' ' && Cmdline[j]!='\0') j++;
	}
	else i=j;

	if(j==i) strcpy(GOBPath, "E:\\");
	else
	{
		strncpy(GOBPath, Cmdline+i, 1);
		GOBPath[1]='\0';
		strcat(GOBPath, ":\\");
	}

	wc.cbSize=sizeof(WNDCLASSEX);
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	wc.hInstance=hInst;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName="OpenGL";
	wc.style=CS_OWNDC;
	wc.lpszMenuName=NULL;

	RegisterClassEx(&wc);
	
	srand(GetTickCount());
	hWnd=CreateWindowEx(0,"OpenGL","OpenGL Console",
		WS_POPUP | WS_VISIBLE,
		0,0,Render::ScreenX,Render::ScreenY,NULL,(HMENU)NULL,hInst,NULL);

    hFPSWindow=CreateWindowEx(WS_EX_TOPMOST,"STATIC", "", WS_POPUP | WS_VISIBLE | SS_CENTER, Render::ScreenX/2-30, 2, 50, 18, NULL, NULL, hInst, NULL);

    Console::Setup(hInst);
	/*
	RECT rect;
	GetClientRect(hWnd, &rect);
	ScreenX=rect.right-rect.left;
	ScreenY=rect.bottom-rect.top;*/

	if(!Render::OpenGL_Initialize(hWnd)) return 0;
    Sound::Initialize(hWnd);
    Game::Initialize();

	ShowCursor(FALSE);
	SetCursorPos(Render::ScreenX/2, Render::ScreenY/2);
	I_Setup(hWnd, hInst);

	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
        Game::GameLoop();
		//if(I_IsInputEnabled()) ShowCursor(FALSE);
	}

	return msg.wParam;
}

void ShowFramerate(int framerate)
{
	char buffer[100];
	HDC hDC2;
	//wsprintf(buffer, "OpenGL Console - %i fps", framerate);
	//SetWindowText(hWnd, buffer);
	//hDC2=GetDC(hFPSWindow);
	wsprintf(buffer,"%i fps",framerate);
	SetWindowText(hFPSWindow, buffer);
	//TextOut(hDC2,ScreenX/2-30,2,buffer,strlen(buffer));

	//ReleaseDC(NULL,hDC2);

}

