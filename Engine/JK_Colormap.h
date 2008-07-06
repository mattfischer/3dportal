#ifndef JK_COLORMAP_H
#define JK_COLORMAP_H

#include <windows.h>

#include "U_Collection.h"

struct JK_C_RGBTriple {
	char r;
	char g;
	char b;
};

struct JK_CMP {
	char name[4];
	long unk0;
	long transp;
	char unk1[52];
	JK_C_RGBTriple palette[256];
};

struct JK_Colormap : public U_NameableItem
{
public:
	JK_Colormap( const string& filename );
	
	JK_C_RGBTriple Palette(UCHAR i);

protected:
	JK_CMP cmp;
};

#endif