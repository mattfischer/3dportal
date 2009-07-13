#ifndef R_SPRITE_H
#define R_SPRITE_H

#include "U_Collection.h"

#include "R_Texture.h"
#include "M_Vector.h"

class R_Sprite : public U_NameableItem
{
public:
	// JK_Sprite.cpp
	R_Sprite( const string& filename );

	int NumFrames();

	// R_Sprite.cpp
	void Draw( int frame );

protected:
	R_Texture *texture;
	int type;
	float width;
	float height;
	int geo;
	int light;
	int tex;
	float extraLight;
	M_Vector offset;
};

#endif