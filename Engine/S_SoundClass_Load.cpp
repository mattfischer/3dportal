#include "S_SoundClass.h"
#include "S_Sound.h"

#include "JK_Parse.h"
#include "JK_Level.h"
#include "JK_GOB.h"

namespace Sound
{
    Class::Class( const string& filename )
    {
	    int i, j, n;
	    bool error;
	    string text;
	    string data;
	    string fullname;

	    fullname = "misc\\snd\\" + filename;

        data = Jk::Gob::getFile( fullname );
        Jk::Parser parser(data);

	    numEntries = 0;
	    while( 1 )
	    {
            parser.getLine(error);
		    if( error ) break;
		    numEntries++;
	    }

	    entries = new Entry[numEntries];

	    parser.reset();

	    for( n = 0 ; n < numEntries ; n++ )
	    {
            Jk::Parser::Line line = parser.getLine( error );
    		
		    entries[n].name = line.getString( error );
		    text = line.getString( error );
		    entries[n].flags = line.getHex( error );
		    if( error )
			    entries[n].flags = 0;
		    entries[n].minDist = line.getFloat( error );
		    if( error )
			    entries[n].minDist = -1;
		    entries[n].maxDist = line.getFloat( error );
		    if( error )
			    entries[n].maxDist = -1;
		    entries[n].volume = line.getFloat( error );
		    if( error )
			    entries[n].volume = 1;

		    entries[n].sound = currentLevel.sounds[text];
	    }
    }
}