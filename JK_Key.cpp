#include "JK_Key.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

#include "U_Lowercase.h"

namespace Jk
{
    Key::Key( const string& filename )
    {
	    string fullname;
        Jk::Parser::Line line;
	    string data;
	    bool error;
	    int i, j;
	    float f1, f2, f3;

	    name = filename;
    	
	    fullname = "3do\\key\\" + name;
        data = Jk::Gob::getFile( fullname );
        Jk::Parser parser(data);

	    parser.findString( "SECTION: HEADER", error );
	    line = parser.getLine( error );

	    line.matchString( "FLAGS", error );
	    flags = line.getInt( error );

	    line = parser.getLine( error );
	    line.matchString( "TYPE", error );
	    type = line.getHex( error );

	    line = parser.getLine( error );
	    line.matchString( "FRAMES", error );
	    numFrames = line.getInt( error );

	    line = parser.getLine( error );
	    line.matchString( "FPS", error );
	    fps = line.getInt( error );

	    line = parser.getLine( error );
	    line.matchString( "JOINTS", error );
	    joints = line.getInt( error );

	    numMarkers = 0;
	    line = parser.getLine( error );
	    line.matchString( "SECTION: MARKERS", error );

	    if( !error )
	    {
		    line = parser.getLine( error );
		    line.matchString( "MARKERS", error );
		    numMarkers = line.getInt( error );

		    markers = new Marker[numMarkers];
		    for( i = 0 ; i < numMarkers ; i++ )
		    {
			    line = parser.getLine( error );

			    markers[i].frame = line.getFloat( error );
			    markers[i].type = line.getInt( error );
		    }

		    line = parser.getLine( error );
	    }	

	    line.matchString( "SECTION: KEYFRAME NODES", error );
	    line = parser.getLine( error );
	    line.matchString( "NODES", error );
	    numNodes = line.getInt( error );

	    nodes = new Node[numNodes];

	    for( i = 0 ; i < numNodes ; i++ )
	    {
		    parser.getLine( error );
		    line = parser.getLine( error );

		    line.matchString( "MESH NAME", error );
		    nodes[i].name = Util::Lowercase( line.getString( error ) );

		    line = parser.getLine( error );
		    line.matchString( "ENTRIES", error );
		    nodes[i].numEntries = line.getInt( error );

		    nodes[i].entries = new Frame[nodes[i].numEntries];
		    for( j = 0 ; j < nodes[i].numEntries ; j++ )
		    {
			    line = parser.getLine( error );
    		
			    line.getInt( error );
			    nodes[i].entries[j].frame = line.getInt( error );
			    nodes[i].entries[j].flags = line.getHex( error );
			    f1 = line.getFloat( error );
			    f2 = line.getFloat( error );
			    f3 = line.getFloat( error );
			    nodes[i].entries[j].position = Math::Vector( f1, f2, f3 );
    			
			    f1 = line.getFloat( error );
			    f2 = line.getFloat( error );
			    f3 = line.getFloat( error );
			    nodes[i].entries[j].orientation = Math::Vector( f1, f2, f3 );

			    line = parser.getLine( error );
                f1 = line.getFloat( error );
			    f2 = line.getFloat( error );
			    f3 = line.getFloat( error );
			    nodes[i].entries[j].deltaPosition = Math::Vector( f1, f2, f3 );
    			
			    f1 = line.getFloat( error );
			    f2 = line.getFloat( error );
			    f3 = line.getFloat( error );
			    nodes[i].entries[j].deltaOrientation = Math::Vector( f1, f2, f3 );
		    }
	    }
    }

    void Key::interpolateFrame( const string &node, float time, int flags, Math::Vector &position, Math::Vector &orientation )
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