#ifndef R_TEXTURE_H
#define R_TEXTURE_H

#include <string>
#include <windows.h>

#include "U_VectorMap.h"

namespace Jk
{
    class Colormap;
}

using std::string;

namespace Render
{
    class Texture
    {
    public:
	    Texture( const string& filename );

        void Register( Util::VectorMap<Jk::Colormap*> &colormaps );
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

    void Texture_Register( Texture *textures, int numTextures );
}

#endif