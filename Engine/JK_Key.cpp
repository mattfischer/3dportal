#include "JK_Key.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

JK_Key::JK_Key( const string& filename )
{
	string fullname;
	string line;
	string data;
	char *dataPointer;
	int size;
	int error;
	int pos, pos2;
	int i, j;
	float f1, f2, f3;

	name = filename;
	
	fullname = "3do\\key\\" + name;
	JK_GOB_GetFile( fullname, &dataPointer, &size );
	data = dataPointer;
	delete[] dataPointer;

	pos = 0;

	JKP_FindString( data, pos, size, "SECTION: HEADER", error );
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "FLAGS", error );
	flags = JKP_GetInt( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "TYPE", error );
	type = JKP_GetHex( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "FRAMES", error );
	numFrames = JKP_GetInt( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "FPS", error );
	fps = JKP_GetInt( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "JOINTS", error );
	joints = JKP_GetInt( line, pos2, error );

	numMarkers = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "SECTION: MARKERS", error );
	if( !error )
	{
		error = 0;

		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		JKP_MatchString( line, pos2, "MARKERS", error );
		numMarkers = JKP_GetInt( line, pos2, error );

		markers = new JK_K_Marker[numMarkers];
		for( i = 0 ; i < numMarkers ; i++ )
		{
			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;

			markers[i].frame = JKP_GetFloat( line, pos2, error );
			markers[i].type = JKP_GetInt( line, pos2, error );
		}

		line = JKP_GetNonEmptyLine( data, pos, size, error );
	}	

	JKP_MatchString( line, pos2, "SECTION: KEYFRAME NODES", error );
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "NODES", error );
	numNodes = JKP_GetInt( line, pos2, error );

	nodes = new JK_K_Node[numNodes];

	for( i = 0 ; i < numNodes ; i++ )
	{
		JKP_GetNonEmptyLine( data, pos, size, error );
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		JKP_MatchString( line, pos2, "MESH NAME", error );
		nodes[i].name = JKP_GetString( line, pos2, error );

		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		JKP_MatchString( line, pos2, "ENTRIES", error );
		nodes[i].numEntries = JKP_GetInt( line, pos2, error );

		nodes[i].entries = new JK_K_Frame[nodes[i].numEntries];
		for( j = 0 ; j < nodes[i].numEntries ; j++ )
		{
			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
		
			JKP_GetInt( line, pos2, error );
			nodes[i].entries[j].frame = JKP_GetInt( line, pos2, error );
			nodes[i].entries[j].flags = JKP_GetHex( line, pos2, error );
			f1 = JKP_GetFloat( line, pos2, error );
			f2 = JKP_GetFloat( line, pos2, error );
			f3 = JKP_GetFloat( line, pos2, error );
			nodes[i].entries[j].position = M_Vector( f1, f2, f3 );
			
			f1 = JKP_GetFloat( line, pos2, error );
			f2 = JKP_GetFloat( line, pos2, error );
			f3 = JKP_GetFloat( line, pos2, error );
			nodes[i].entries[j].orientation = M_Vector( f1, f2, f3 );

			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
		}
	}
}

void JK_Key::interpolateFrame( const string &node, float time, M_Vector &position, M_Vector &orientation )
{
	int i;
	int frame;
	int e;
	float t;

	for( i = 0 ; i < numNodes ; i++ )
	{
		if( nodes[i].name == node ) break;
	}

	if( i == numNodes ) return;

	frame = time * fps;
	while(frame>numFrames) frame -= numFrames;

	for( e = 0 ; e < nodes[i].numEntries - 1 ; e++ )
	{
		if( nodes[i].entries[e].frame <= frame && nodes[i].entries[e+1].frame > frame )
		{
			t = ( frame - nodes[i].entries[e].frame ) / ( nodes[i].entries[e+1].frame - nodes[i].entries[e].frame );
			position = nodes[i].entries[e].position + ( nodes[i].entries[e+1].position - nodes[i].entries[e].position ) * t;
			orientation = nodes[i].entries[e].orientation + ( nodes[i].entries[e+1].orientation - nodes[i].entries[e].orientation ) * t;
			return;
		}
	}
}
