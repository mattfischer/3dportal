#include "G_Console.h"
#include "I_Input.h"

#include <windows.h>

HWND hConsole;
HWND hTextWindow;

bool visible;
char buffer[16000];

CRITICAL_SECTION cs;

namespace Console
{
    LRESULT CALLBACK ConsoleProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
	    return DefWindowProc( hWnd, msg, wParam, lParam );
    }

    void Setup( HINSTANCE hInst )
    {
	    WNDCLASSEX wc;
	    RECT rect;

	    wc.cbSize = sizeof( WNDCLASSEX );
	    wc.cbClsExtra = 0;
	    wc.cbWndExtra = 0;
	    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	    wc.hInstance = hInst;
	    wc.lpfnWndProc = ConsoleProc;
	    wc.lpszClassName = "Console";
	    wc.style = CS_OWNDC;
	    wc.lpszMenuName = NULL;

	    RegisterClassEx( &wc );
    	
	    hConsole = CreateWindowEx( WS_EX_TOPMOST, "Console", "Console Window", WS_POPUP | WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInst, NULL );

	    GetClientRect( hConsole, &rect );
	    hTextWindow = CreateWindow( "EDIT", NULL, WS_CHILD |
	    WS_BORDER | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL | WS_VISIBLE,
	    0, 0, rect.right - rect.left, rect.bottom - rect.top, hConsole, NULL, hInst, NULL );
	    buffer[0] = '\0';
	    visible = false;

	    InitializeCriticalSection( &cs );
    }

    void Shutdown()
    {
	    DestroyWindow( hConsole );
    }

    void Show()
    {
	    ShowWindow( hConsole, SW_SHOW );
	    UpdateWindow( hConsole );
	    visible = true;
    }

    void Hide()
    {
	    ShowWindow( hConsole, SW_HIDE );
	    visible = false;
    }

    void ToggleVisible()
    {
        if( visible ) Console::Hide();
	    else		  Console::Show();
    }

    void Out( const string& message )
    {
	    int numLines;

	    EnterCriticalSection(&cs);

	    SetWindowText( hTextWindow, ( message + "\r\n" ).c_str() );
	    //if(!visible) CON_Show();

	    numLines = SendMessage( hTextWindow, EM_GETLINECOUNT, 0, 0 );
	    SendMessage( hTextWindow, EM_LINESCROLL, 0, numLines );
	    UpdateWindow( hConsole );
	    LeaveCriticalSection( &cs );
    }
}