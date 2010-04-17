#include "JK_Colormap.h"
#include "JK_GOB.h"

namespace Jk
{
    Colormap::Colormap( const string& filename )
    {
	    char *data;
	    int size;
	    string fullname;

	    fullname = "misc\\cmp\\" + filename;
    	
        Jk::Gob::getFile( fullname, &data, &size );

	    memcpy( &cmp, data, sizeof(CMP) );

	    delete[] data;
    }

    Colormap::RGBTriple Colormap::Palette(UCHAR i)
    {
	    return cmp.palette[i];
    }
}
