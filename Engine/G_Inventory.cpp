#include "G_Inventory.h"
#include "JK_Parse.h"
#include "JK_GOB.h"
#include "JK_Level.h"
#include "C_Script.h"

G_Item G_Items[200];
/*
void G_SetupInventory()
{
	int i;

	for(i=0;i<200;i++)
		G_Items[i].value=0;

	G_Items[0].max=0;
	G_Items[1].max=1;
	G_Items[2].max=1;
	G_Items[3].max=1;
	G_Items[4].max=30;
	G_Items[5].max=1;
	G_Items[6].max=1;
	G_Items[7].max=1;
	G_Items[8].max=30;
	G_Items[9].max=1;
	G_Items[10].max=1;

	G_Items[11].max=500;
	G_Items[12].max=500;
	G_Items[13].max=200;
	G_Items[14].max=400;
	G_Items[15].max=30;
	
	G_Items[16].max=32;
	G_Items[17].max=32;
	G_Items[18].max=2;

	G_Items[20].max=4;
	G_Items[21].max=4;
	G_Items[22].max=4;
	G_Items[23].max=4;
	G_Items[24].max=4;
	G_Items[25].max=4;
	G_Items[26].max=4;
	G_Items[27].max=4;
	G_Items[28].max=4;
	G_Items[29].max=4;
	G_Items[30].max=4;

	G_Items[31].max=4;
	G_Items[32].max=4;
	G_Items[33].max=4;
	G_Items[34].max=4;

	G_Items[40].max=5;
	G_Items[41].max=1;
	G_Items[42].max=1;
	G_Items[43].max=1;
	G_Items[44].max=1;
	G_Items[45].max=1;
	G_Items[46].max=1;
	G_Items[47].max=1;
	G_Items[48].max=1;
	G_Items[49].max=1;
	G_Items[50].max=1;
	G_Items[51].max=1;

	G_Items[60].max=200;
	G_Items[61].max=200;
	G_Items[62].max=1;
	G_Items[63].max=2;
	G_Items[64].max=1;
	G_Items[65].max=1;
	G_Items[66].max=1;

	
	G_Items[70].max=20;
	G_Items[71].max=20;
	G_Items[72].max=1000;
	G_Items[73].max=1000;
	G_Items[74].max=40;
	G_Items[75].max=100;

	G_Items[80].max=2;
	G_Items[81].max=1;
	G_Items[82].max=1;
	G_Items[83].max=1;
	G_Items[84].max=1;

	G_Items[99].max=99999;
	G_Items[100].max=1000;
	G_Items[101].max=1000;
	G_Items[102].max=1000;
	G_Items[103].max=1000;
	G_Items[104].max=1000;
	G_Items[105].max=1000;
	G_Items[106].max=1000;
	G_Items[107].max=1000;
	G_Items[108].max=1000;
	G_Items[109].max=1000;
	G_Items[110].max=1000;
	G_Items[111].max=1000;
	G_Items[112].max=1000;
	G_Items[113].max=1000;
	G_Items[114].max=1000;
	G_Items[115].max=1000;

	G_Items[150].max=99999;
}*/

void G_SetupInventory()
{
	string data;
	char *dataPointer;
	string line;
	string text;
	int size;
	int pos, pos2;
	int id;
	int error;


	JK_GOB_GetFile( "misc\\items.dat", &dataPointer, &size );
	data = dataPointer;
	delete[] dataPointer;

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
}
	