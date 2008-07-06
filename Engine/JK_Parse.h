#ifndef JK_PARSE_H
#define JK_PARSE_H

#include "Global.h"

#include "M_Vector.h"

#include <string>

using std::string;

string JKP_GetNonEmptyLine( const string& input, int& start, int size, int& error );
float JKP_GetFloat( const string& input, int& start, int& error );
int JKP_GetInt( const string& input, int& start, int& error );
string JKP_GetString( const string& input, int& start, int& error );
void JKP_FindString( const string& input, int& start, int size, const string& match, int& error );
void JKP_MatchString( const string& input, int& start, const string& match, int& error );
int JKP_GetHex( const string& input, int& start, int& error );

#endif