#ifndef I_INPUT_H
#define I_INPUT_H

#include <windows.h>

namespace Input
{
    void Process( float time );
    void EnableInput( bool enable );
    bool IsInputEnabled();
    void Setup( HWND hWnd, HINSTANCE hInst );
}
#endif