#include "R_Texture.h"

#include "JK_GOB.h"

struct JK_MAT_Header {
	char name[4];
	long ver;
	long Type;
	long NumOfTextures;
	long NumOfTextures1;
	long unk0;
	long unk1;
	long unk2[12];
};

struct JK_MAT_ColorHeader {
	long textype;
	long colornum;
	long unk0[4];
};

struct JK_MAT_THeader {
	long textype;
	long colornum;
	long unk0[4];
	long unk1[2];
	long unk2;
	long CurrentTXNum;
};

struct JK_MAT_TextureData {
	long SizeX;
	long SizeY;
	long Pad[3];
	long NumMipMaps;
};

namespace Render
{
    Texture::Texture( const string& filename )
    {
	    JK_MAT_Header header;
	    JK_MAT_THeader theader;
	    JK_MAT_ColorHeader colorHeader;
	    JK_MAT_TextureData textureData;
	    char *fileData;
	    int i;
	    int size;
	    int cursor;
	    string fullname;
	    int found;
	    int n;
	    int mip;

	    fullname = "mat\\" + filename;
    	
        found = Jk::Gob::getFile( fullname, &fileData, &size );
	    if( found == 0 )
	    {
		    fullname = "3do\\mat\\" + filename;
    	
            found = Jk::Gob::getFile( fullname, &fileData, &size );
	    }

	    cursor = 0;
	    memcpy( &header, fileData + cursor, sizeof( JK_MAT_Header ) );
	    cursor += sizeof( JK_MAT_Header );

	    if( header.Type == 0 )
	    {
		    sizeX = 1;
		    sizeY = 1;
		    numCels = 1;
		    memcpy( &colorHeader, fileData + cursor, sizeof( JK_MAT_ColorHeader ) );
		    data = new UCHAR*[numCels];
		    data[0] = new UCHAR[1];
		    data[0][0] = colorHeader.colornum;
		    delete[] fileData;
		    return;
	    }

	    memcpy( &theader, fileData + cursor, sizeof( JK_MAT_THeader ) );
	    cursor += sizeof( JK_MAT_THeader )*header.NumOfTextures;

	    numCels = header.NumOfTextures;
	    data = new UCHAR*[numCels];

	    for( n = 0 ; n < numCels ; n++ )
	    {
		    memcpy( &textureData, fileData + cursor, sizeof( JK_MAT_TextureData ) );
		    cursor += sizeof( JK_MAT_TextureData );

		    sizeX = textureData.SizeX;
		    sizeY = textureData.SizeY;
    		
		    transparent = false;
		    for( i = 0 ; i < sizeX * sizeY ; i++ )
			    if( fileData[cursor+i] == 0 )
			    {
				    transparent = true;
				    break;
			    }

		    data[n] = new UCHAR[sizeX * sizeY];
		    memcpy( data[n], fileData + cursor, sizeX * sizeY );
    	
		    mip = 1;
		    for( i = 0 ; i < textureData.NumMipMaps ; i++ )
		    {
			    cursor += sizeX * sizeY / ( mip * mip );
			    mip *= 2;
		    }
	    }
	    delete[] fileData;
    }
}