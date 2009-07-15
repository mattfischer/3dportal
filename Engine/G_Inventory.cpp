#include "G_Inventory.h"
#include "JK_Parse.h"
#include "JK_GOB.h"
#include "JK_Level.h"
#include "C_Script.h"

G_Item G_Items[200];

void G_SetupInventory()
{
	string data;
	string line;
	string text;
	int pos, pos2;
	int id;
	int error;
    int size;

    data = Jk::Gob::getFile( "misc\\items.dat" );
    size = data.size();

	pos = 0;
	while( 1 )
	{
		error = 0;
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		if( error ) break;

		pos2 = 0;

		text = JKP_GetString( line, pos2, error );

		id = JKP_GetInt( line, pos2, error );

		G_Items[id].min = JKP_GetFloat( line, pos2, error );
		G_Items[id].value = G_Items[id].min;
		G_Items[id].max = JKP_GetFloat( line, pos2, error );
		G_Items[id].flags = JKP_GetHex( line, pos2, error );

		text = JKP_GetString( line, pos2, error );
		if( error == 0 && text[0] != '#' )
		{
			G_Items[id].cog = new C_Script( text.substr( 4 ) );
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
