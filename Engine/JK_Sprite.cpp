#include "JK_Level.h"
#include "R_Sprite.h"
#include "JK_Parse.h"
#include "JK_GOB.h"

R_Sprite::R_Sprite( const string &filename )
{
	string fullname;
    Jk::Parser::Line line;
	string data;
	bool error;

	name = filename;
	
	fullname = "misc\\spr\\" + filename;

    data = Jk::Gob::getFile( fullname );
    Jk::Parser parser(data);

	line = parser.getLine( error );

	string mat = line.getString( error );
	texture = currentLevel.textures[mat];

	type = line.getInt( error );
	width = line.getFloat( error );
	height = line.getFloat( error );
	geo = line.getInt( error );
	light = line.getInt( error );
	tex = line.getInt( error );
	extraLight = line.getFloat( error );
	float x, y, z;
	x = line.getFloat( error );
	y = line.getFloat( error );
	z = line.getFloat( error );
	offset = M_Vector( x, y, z );
}

int R_Sprite::NumFrames()
{
	return texture->NumCels();
}