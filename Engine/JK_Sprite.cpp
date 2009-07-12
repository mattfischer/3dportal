#include "JK_Level.h"
#include "R_Sprite.h"
#include "JK_Parse.h"
#include "JK_GOB.h"

R_Sprite::R_Sprite( const string &filename )
{
	string fullname;
	string line;
	string data;
	char *dataPointer;
	int size;
	int error;
	int pos, pos2;

	name = filename;
	
	fullname = "misc\\spr\\" + filename;

	JK_GOB_GetFile( fullname, &dataPointer, &size );
	data = dataPointer;
	delete[] dataPointer;

	pos = 0;

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;

	string mat = JKP_GetString( line, pos2, error );
	texture = currentLevel.textures[mat];

	type = JKP_GetInt( line, pos2, error );
	width = JKP_GetFloat( line, pos2, error );
	height = JKP_GetFloat( line, pos2, error );
	geo = JKP_GetInt( line, pos2, error );
	light = JKP_GetInt( line, pos2, error );
	tex = JKP_GetInt( line, pos2, error );
	extraLight = JKP_GetFloat( line, pos2, error );
	float x, y, z;
	x = JKP_GetFloat( line, pos2, error );
	y = JKP_GetFloat( line, pos2, error );
	z = JKP_GetFloat( line, pos2, error );
	offset = M_Vector( x, y, z );
}