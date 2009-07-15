#include "S_SoundClass.h"
#include "S_Sound.h"

#include "JK_Parse.h"
#include "JK_Level.h"
#include "JK_GOB.h"

S_SoundClass::S_SoundClass( const string& filename )
{
	int i, j, n;
	int pos, pos2, error;
	string line;
	string text;
	string data;
	string fullname;
    int size;

	name = filename;
	
	fullname = "misc\\snd\\" + filename;

    data = Jk::Gob::getFile( fullname );
    size = data.size();

	pos = 0;
	numEntries = 0;
	while( 1 )
	{
		error = 0;
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		if( error == 1 ) break;
		numEntries++;
	}

	entries = new S_ClassEntry[numEntries];

	pos = 0;
	for( n = 0 ; n < numEntries ; n++ )
	{
		error = 0;
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		entries[n].name = JKP_GetString( line, pos2, error );
		text = JKP_GetString( line, pos2, error );
		entries[n].flags = JKP_GetHex( line, pos2, error );
		if( error )
			entries[n].flags = 0;
		entries[n].minDist = JKP_GetFloat( line, pos2, error );
		if( error )
			entries[n].minDist = -1;
		entries[n].maxDist = JKP_GetFloat( line, pos2, error );
		if( error )
			entries[n].maxDist = -1;
		entries[n].volume = JKP_GetFloat( line, pos2, error );
		if( error )
			entries[n].volume = 1;

		entries[n].sound = currentLevel.sounds[text];
	}
}