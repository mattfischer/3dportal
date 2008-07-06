#ifndef I_INPUT_H
#define I_INPUT_H

#include <windows.h>

void I_Process( float time );
void I_EnableInput( bool enable );
bool I_IsInputEnabled();
void I_Setup( HWND hWnd, HINSTANCE hInst );

#endif