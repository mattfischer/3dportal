#include "JK_Colormap.h"
#include "JK_GOB.h"

JK_Colormap::JK_Colormap( const string& filename )
{
	char *data;
	int size;
	string fullname;

	fullname = "misc\\cmp\\" + filename;
	
    Jk::Gob::getFile( fullname, &data, &size );

	memcpy( &cmp, data, sizeof(JK_CMP) );

	delete[] data;

	name = filename;
}
/*
void JK_Colormap::SetNum(int n)
{

}

char *JK_Colormap::GetName()
{
	return name;
}
*/
JK_C_RGBTriple JK_Colormap::Palette(UCHAR i)
{
	return cmp.palette[i];
}

