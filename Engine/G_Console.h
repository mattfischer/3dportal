#ifndef G_CONSOLE_H
#define G_CONSOLE_H

#include <windows.h>

#include <string>

using std::string;

void CON_Setup( HINSTANCE hInst );
void CON_Shutdown();

void CON_Out( const string& message );

void CON_Show();
void CON_Hide();
void CON_ToggleVisible();

#endif