#include "JK_GOB.h"

#include <windows.h>
#include <cctype>
#include <algorithm>

struct JK_GOB_Header {
	char name[3];
	char version;
	long firstFileSize;
	long numItemsOffset;
	long numItems;
};

struct JK_GOB_Item {
	long offset;
	long length;
	char filename[128];
};

int numEpisodeItems;
JK_GOB_Item *episodeItems;
int numResource1Items;
JK_GOB_Item *resource1Items;
int numResource2Items;
JK_GOB_Item *resource2Items;

HANDLE episodeGOB;
HANDLE resource1GOB;
HANDLE resource2GOB;

extern char GOBPath[];

namespace Jk
{
    void Gob::init()
    {
	    JK_GOB_Header header;
	    DWORD dummy;
	    string buffer;

	    buffer = string( GOBPath ) + "GAMEDATA\\EPISODE\\JK1.GOB";
	    episodeGOB = CreateFile( buffer.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	    ReadFile( episodeGOB, &header, sizeof( JK_GOB_Header ), &dummy, NULL );
	    numEpisodeItems = header.numItems;
	    episodeItems = new JK_GOB_Item[numEpisodeItems];
	    ReadFile( episodeGOB, episodeItems, sizeof( JK_GOB_Item ) * numEpisodeItems, &dummy, NULL );
    	
	    buffer = string( GOBPath ) + "GAMEDATA\\MININSTALL\\RES1HI.GOB";
	    resource1GOB = CreateFile( buffer.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	    ReadFile( resource1GOB, &header, sizeof( JK_GOB_Header ), &dummy, NULL );
	    numResource1Items = header.numItems;
	    resource1Items = new JK_GOB_Item[numResource1Items];
	    ReadFile( resource1GOB, resource1Items, sizeof( JK_GOB_Item ) * numResource1Items, &dummy, NULL );

	    buffer = string( GOBPath ) + "GAMEDATA\\RESOURCE\\RES2.GOB";
	    resource2GOB = CreateFile( buffer.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	    ReadFile( resource2GOB, &header, sizeof( JK_GOB_Header ), &dummy, NULL );
	    numResource2Items = header.numItems;
	    resource2Items = new JK_GOB_Item[numResource2Items];
	    ReadFile( resource2GOB, resource2Items, sizeof( JK_GOB_Item ) * numResource2Items, &dummy, NULL );
    }

    void Gob::close()
    {
	    CloseHandle(episodeGOB);
	    CloseHandle(resource1GOB);
	    CloseHandle(resource2GOB);
    }

    int Gob::getFile(const string& filename, char **data, int *size)
    {
	    int i;
	    HANDLE file;
	    int index;
	    JK_GOB_Item item;
	    DWORD dummy;
	    string file1, file2;
	    index = -1;

	    for(i = 0; i < numEpisodeItems; i++)
	    {
		    if(!strcmpi(episodeItems[i].filename, filename.c_str()))
		    {
			    file = episodeGOB;
			    item = episodeItems[i];
			    index = i;
			    break;
		    }
	    }

	    if(index == -1)
	    {
		    for(i = 0; i < numResource1Items; i++)
		    {
			    if(!strcmpi(resource1Items[i].filename, filename.c_str()))
			    {
				    file = resource1GOB;
				    item = resource1Items[i];
				    index = i;
				    break;
			    }
		    }
	    }

	    if(index == -1)
	    {
		    for(i = 0; i < numResource2Items; i++)
		    {
			    if(!strcmpi(resource2Items[i].filename, filename.c_str()))
			    {
				    file = resource2GOB;
				    item = resource2Items[i];
				    index = i;
				    break;
			    }
		    }
	    }

	    if(index == -1) return 0;

	    *data = new char[item.length + 1];
	    if(size != NULL) 
		    *size = item.length;

	    SetFilePointer(file, item.offset, NULL, FILE_BEGIN);

	    ReadFile(file, *data, item.length, &dummy, NULL);
	    (*data)[item.length]='\0';

	    return 1;
    }

    string Gob::getFile(const string &filename)
    {
        char *data;
        int size;
        string s;

        getFile(filename, &data, &size);
        s = data;
        delete[] data;

        return s;
    }
}