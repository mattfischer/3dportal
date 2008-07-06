#ifndef JK_GOB_H
#define JK_GOB_H

#include "Global.h"

#include <string>

using std::string;

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

void JK_GOB_OpenFiles();
void JK_GOB_CloseFiles();
int JK_GOB_GetFile( const string& filename, char **data, int *size);

#endif
