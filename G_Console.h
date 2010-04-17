#ifndef G_CONSOLE_H
#define G_CONSOLE_H

#include <windows.h>

#include <string>

using std::string;

namespace Console
{
    void Setup( HINSTANCE hInst );
    void Shutdown();

    void Out( const string& message );

    void Show();
    void Hide();
    void ToggleVisible();
}

#endif