#include "G_Inventory.h"
#include "JK_Parse.h"
#include "JK_GOB.h"
#include "JK_Level.h"
#include "C_Script.h"

G_Item G_Items[200];

void G_SetupInventory()
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

		G_Items[id].min = line.getFloat( error );
		G_Items[id].value = G_Items[id].min;
		G_Items[id].max = line.getFloat( error );
		G_Items[id].flags = line.getHex( error );

		text = line.getString( error );
		if( error == 0 && text[0] != '#' )
		{
            G_Items[id].cog = new Cog::Script( text.substr( 4 ) );
			G_Items[id].cog->Message( "startup", -1, 0, 0 );
		}
	}

	G_Items[11].value = 100;
}

void G_ActivateBin( int bin )
{
    if( G_Items[bin].cog )
    {
        G_Items[bin].cog->Message( "selected", -1, 0, currentLevel.playerNum );
    }
}

void G_DeactivateBin( int bin )
{
    if( G_Items[bin].cog )
    {
        G_Items[bin].cog->Message( "deselected", -1, 0, currentLevel.playerNum );
    }
}
