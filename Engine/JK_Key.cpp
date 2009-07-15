#include "JK_Key.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

#include "U_Lowercase.h"

namespace Jk
{
    Key::Key( const string& filename )
    {
	    string fullname;
	    string line;
	    string data;
	    int error;
	    int pos, pos2;
	    int i, j;
	    float f1, f2, f3;
        int size;

	    name = filename;
    	
	    fullname = "3do\\key\\" + name;
        data = Jk::Gob::getFile( fullname );
        size = data.size();

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

		    markers = new Marker[numMarkers];
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

	    nodes = new Node[numNodes];

	    for( i = 0 ; i < numNodes ; i++ )
	    {
		    JKP_GetNonEmptyLine( data, pos, size, error );
		    line = JKP_GetNonEmptyLine( data, pos, size, error );
		    pos2 = 0;
		    JKP_MatchString( line, pos2, "MESH NAME", error );
		    nodes[i].name = U_Lowercase( JKP_GetString( line, pos2, error ) );

		    line = JKP_GetNonEmptyLine( data, pos, size, error );
		    pos2 = 0;
		    JKP_MatchString( line, pos2, "ENTRIES", error );
		    nodes[i].numEntries = JKP_GetInt( line, pos2, error );

		    nodes[i].entries = new Frame[nodes[i].numEntries];
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
                f1 = JKP_GetFloat( line, pos2, error );
			    f2 = JKP_GetFloat( line, pos2, error );
			    f3 = JKP_GetFloat( line, pos2, error );
			    nodes[i].entries[j].deltaPosition = M_Vector( f1, f2, f3 );
    			
			    f1 = JKP_GetFloat( line, pos2, error );
			    f2 = JKP_GetFloat( line, pos2, error );
			    f3 = JKP_GetFloat( line, pos2, error );
			    nodes[i].entries[j].deltaOrientation = M_Vector( f1, f2, f3 );
		    }
	    }
    }

    void Key::interpolateFrame( const string &node, float time, int flags, M_Vector &position, M_Vector &orientation )
    {
	    int i;
	    float frame;
	    int e;
	    float t;

	    for( i = 0 ; i < numNodes ; i++ )
	    {
		    if( nodes[i].name == node ) break;
	    }

	    if( i == numNodes ) return;

	    frame = time * fps;
	    if( frame > numFrames )
        {
            switch( flags )
            {
            case 0:
                frame -= ((int)(frame / numFrames)) * numFrames;
                break;
                
            case 0x14:
            default:
                frame = numFrames - 1;
                break;
            }
        }
        
	    for( e = 0 ; e < nodes[i].numEntries ; e++ )
	    {
		    if( nodes[i].entries[e].frame <= frame && (e == nodes[i].numEntries - 1 || nodes[i].entries[e+1].frame > frame ))
		    {
			    t = ( frame - nodes[i].entries[e].frame );
			    position = nodes[i].entries[e].position + nodes[i].entries[e].deltaPosition * t;
			    orientation = nodes[i].entries[e].orientation + nodes[i].entries[e].deltaOrientation * t;
                if(orientation.x < 0) orientation.x += 360;
                if(orientation.y < 0) orientation.y += 360;
                if(orientation.z < 0) orientation.z += 360;
                if(orientation.x >= 360) orientation.x -= 360;
                if(orientation.y >= 360) orientation.y -= 360;
                if(orientation.z >= 360) orientation.z -= 360;
			    return;
		    }
	    }
    }
}