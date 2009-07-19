#include "G_Inventory.h"
#include "JK_Parse.h"
#include "JK_GOB.h"
#include "JK_Level.h"
#include "C_Script.h"

namespace Game
{
    Item Items[200];

    void SetupInventory()
    {
	    string data;
        Jk::Parser::Line line;
	    string text;
	    int id;
	    bool error;

        data = Jk::Gob::getFile( "misc\\items.dat" );
        Jk::Parser parser(data);

	    while( 1 )
	    {
		    line = parser.getLine( error );
		    if( error ) break;

		    text = line.getString( error );

		    id = line.getInt( error );

            Items[id].min = line.getFloat( error );
		    Items[id].value = Items[id].min;
		    Items[id].max = line.getFloat( error );
		    Items[id].flags = line.getHex( error );

		    text = line.getString( error );
		    if( error == 0 && text[0] != '#' )
		    {
                Items[id].cog = new Cog::Script( text.substr( 4 ) );
			    Items[id].cog->Message( "startup", -1, 0, 0 );
		    }
	    }

	    Items[11].value = 100;
    }

    void ActivateBin( int bin )
    {
        if( Items[bin].cog )
        {
            Items[bin].cog->Message( "selected", -1, 0, currentLevel.playerNum );
        }
    }

    void DeactivateBin( int bin )
    {
        if( Items[bin].cog )
        {
            Items[bin].cog->Message( "deselected", -1, 0, currentLevel.playerNum );
        }
    }
}
