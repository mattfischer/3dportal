#ifndef JK_COLORMAP_H
#define JK_COLORMAP_H

#include <windows.h>

#include <string>

using std::string;

namespace Jk
{
    struct Colormap 
    {
    public:
        struct RGBTriple {
	        char r;
	        char g;
	        char b;
        };

	    Colormap( const string& filename );
    	
	    RGBTriple Palette(UCHAR i);

    protected:
        struct CMP {
	        char name[4];
	        long unk0;
	        long transp;
	        char unk1[52];
	        RGBTriple palette[256];
        };
	    CMP cmp;
    };
}
#endif