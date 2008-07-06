#ifndef R_TEXTURE_H
#define R_TEXTURE_H

#include <windows.h>

#include "U_Collection.h"
#include "U_VectorMap.h"

template<class T> class U_Collection;
class JK_Colormap;

class R_Texture
{
public:
	R_Texture( const string& filename );

	void Register( U_VectorMap<JK_Colormap*> &colormaps );
	void Select( int colormap, int cel );

	int SizeX();
	int SizeY();
	bool NeedsBlending();
	int NumCels();

protected:
	int sizeX;
	int sizeY;
	unsigned int **names;
	int numCels;
	bool transparent;
	UCHAR **data;
};

void R_Texture_Register( R_Texture *textures, int numTextures );

#endif