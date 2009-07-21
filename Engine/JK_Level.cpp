#include "JK_Level.h"
#include "JK_Parse.h"
#include "JK_Template.h"
#include "JK_GOB.h"
#include "JK_Key.h"
#include "JK_AnimClass.h"
#include "JK_Colormap.h"
#include "S_SoundClass.h"

#include "W_Sector.h"
#include "W_Thing.h"
#include "W_Surface.h"

#include "R_Texture.h"
#include "R_Model.h"
#include "R_Sprite.h"

#include "C_Script.h"

#include "S_Sound.h"

#include "U_Lowercase.h"

namespace Jk
{
    struct Adjoin {
	    int mirror;
	    int flags;
        World::Sector *sector;
        World::Surface *surface;
    };

    char **textureFilenames;

    void Level::Load(const string& name)
    {
	    HANDLE levelFile;
	    int size;
	    string data;
	    char *dataPointer;
	    DWORD dummy;
        Jk::Parser::Line line;
	    string text;
	    string text2;
	    bool error;
	    int i, j;
	    string filename;
	    int index;
	    int numVerts;
	    float x,y,z;
	    int colormap;
	    int start;
	    int found;
	    int numEntries;
	    Math::Vector position, rotation;
	    vector<string> textureNames;

	    World::Thing *newThing;
	    shared_ptr<World::Thing> thing;

	    int numVertices;
	    Math::Vector *vertices;
	    int numTextureVertices;
        Render::TextureVertex *textureVertices;
	    int numAdjoins;
	    Adjoin *adjoins;
    	
	    string fullname;

	    string cogName;
	    string cogSource;
	    int cogSize;
	    char *fileData;
	    int fileSize;

	    bool foundWalkplayer = false;
    	
	    levelFile = CreateFile(name.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	    if( levelFile == INVALID_HANDLE_VALUE )
	    {
		    fullname = "jkl\\" + name;
            data = Jk::Gob::getFile(fullname);
	    }
	    else
	    {
		    size = GetFileSize( levelFile, NULL );
		    dataPointer = new char[size];
		    ReadFile( levelFile, dataPointer, size, &dummy, NULL );
		    CloseHandle( levelFile );

            data = dataPointer;
            delete[] dataPointer;
	    }
    	
        Jk::Parser parser(data);

	    // =====================================================================

	    parser.findString( "SECTION: HEADER", error );
    	
	    line = parser.getLine( error );

        line = parser.getLine( error );
	    line.matchString( "World Gravity", error );
	    gravity = line.getFloat( error );
    	
	    line = parser.getLine( error );
	    line.matchString( "Ceiling Sky Z", error );
	    ceilingSkyZ = line.getFloat( error );

	    line = parser.getLine( error );
	    line.matchString( "Horizon Distance", error );
	    horizonDistance = line.getFloat( error );
    	
	    line = parser.getLine( error );
	    line.matchString( "Horizon Pixels per Rev", error );
	    numPixelsPerRev = line.getFloat( error );

	    line = parser.getLine( error );
	    line.matchString( "Horizon Sky Offset", error );
	    horizonOffsetX = line.getFloat( error );
	    horizonOffsetY = line.getFloat( error );

	    line = parser.getLine( error );
	    line = parser.getLine( error );

	    line = parser.getLine( error );
	    line.matchString( "LOD Distances", error );
	    LODDistances[0] = line.getFloat( error );
	    LODDistances[1] = line.getFloat( error );
	    LODDistances[2] = line.getFloat( error );
	    LODDistances[3] = line.getFloat( error );

	    // =====================================================================

	    parser.findString( "SECTION: SOUNDS", error );

	    line = parser.getLine( error );
	    line.matchString( "World sounds", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    filename = line.getString( error );
		    sounds.push_back( new Sound::Buffer( filename ), filename );
	    }

	    // =====================================================================

	    parser.findString( "SECTION: MATERIALS", error );

	    line = parser.getLine( error );
	    line.matchString( "World materials", error );
	    numEntries = line.getInt( error );
	    textureNames.resize( numEntries );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getFloat( error );

		    filename = Util::Lowercase( line.getString( error ) );
    		
            textures.push_back( new Render::Texture( filename ), filename );
		    textureNames[i] = filename;
	    }

	    // =====================================================================

	    parser.findString( "SECTION: GEORESOURCE", error );

	    line = parser.getLine( error );
	    line.matchString( "World Colormaps", error );
	    numEntries = line.getInt( error );
    //	colormaps.Allocate( numEntries );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.getFloat( error );

		    filename = line.getString( error );
            colormaps.push_back( new Jk::Colormap( filename ), filename );
	    }

	    // -------------------------------------------------------------------------

	    line = parser.getLine( error );
	    line.matchString( "World vertices", error );
	    numVertices = line.getInt( error );
	    vertices = new Math::Vector[numVertices];

	    for( i = 0 ; i < numVertices ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.getFloat( error );

		    vertices[i].x = line.getFloat( error );
		    vertices[i].y = line.getFloat( error );
		    vertices[i].z = line.getFloat( error );
	    }

    	
	    // -------------------------------------------------------------------------
    	
	    line = parser.getLine( error );
	    line.matchString( "World texture vertices", error );
	    numTextureVertices = line.getInt( error );
	    textureVertices = new Render::TextureVertex[numTextureVertices];

	    for( i = 0 ; i < numTextureVertices ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.getFloat( error );

		    textureVertices[i].u = line.getFloat( error );
		    textureVertices[i].v = line.getFloat( error );
	    }


	    // -------------------------------------------------------------------------

	    line = parser.getLine( error );
	    line.matchString( "World adjoins", error );
	    numAdjoins = line.getInt( error );
	    adjoins = new Adjoin[numAdjoins];

	    for( i = 0 ; i < numAdjoins ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.getInt( error );

		    adjoins[i].flags = line.getHex( error );
		    adjoins[i].mirror = line.getInt( error );
	    }


	    // -------------------------------------------------------------------------

    	
	    line = parser.getLine( error );
	    line.matchString( "World surfaces", error );
	    numSurfaces = line.getInt( error );
        surfaces = new World::Surface[numSurfaces];

	    for( i = 0 ; i < numSurfaces ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.getFloat( error );

		    index = line.getInt( error );
		    surfaces[i].num = i;
		    surfaces[i].flags = line.getHex( error );
		    surfaces[i].face = line.getHex( error );
		    surfaces[i].geo = line.getInt( error );
		    line.getFloat( error );
		    line.getFloat( error );

		    surfaces[i].adjnum = line.getInt( error );
		    if( surfaces[i].adjnum != -1 )
		    {
			    surfaces[i].adjoined = 1;
			    surfaces[i].adjoinFlags = adjoins[surfaces[i].adjnum].flags;
		    }
		    else
		    {
			    surfaces[i].adjoined = 0;
			    surfaces[i].adjoinFlags = 0;
		    }
    		
		    line.getFloat( error );

		    numVerts = line.getInt( error );
            surfaces[i].polygon = World::Poly( numVerts, 4 );

		    if( index != -1 ) surfaces[i].polygon.SetTexture( textures[textureNames[index]] );
		    else   		      surfaces[i].polygon.SetTexture( NULL );

		    for( j = 0 ; j < numVerts ; j++ )
		    {
			    index = line.getInt( error );
			    surfaces[i].polygon[j].position = vertices[index];
    			
			    index = line.getInt( error );
			    if( surfaces[i].polygon.GetTexture() != NULL ) 
			    {
				    surfaces[i].polygon[j].texture = textureVertices[index];
				    //surfaces[i].polygon[j].texture.u/=surfaces[i].polygon.GetTexture()->sizeX;
				    //surfaces[i].polygon[j].texture.v/=surfaces[i].polygon.GetTexture()->sizeY;
			    }
		    }	

		    for( j = 0 ; j < numVerts ; j++ ) surfaces[i].polygon[j].intensity = line.getFloat( error );
	    }

	    // -------------------------------------------------------------------------


	    for( i = 0 ; i < numSurfaces ; i++ )
	    {
		    line = parser.getLine( error );

		    line.getFloat( error );
		    x = line.getFloat( error );
		    y = line.getFloat( error );
		    z = line.getFloat( error );
    		
		    surfaces[i].polygon.SetPlane( Math::Vector( x, y, z) );
	    }


	    // =====================================================================

	    parser.findString( "Section: SECTORS", error );

	    line = parser.getLine( error );
	    line.matchString( "World sectors", error );
	    numSectors = line.getInt( error );
        sectors = new World::Sector[numSectors];
    	
	    for( i = 0 ; i < numSectors ; i++ )
	    {
		    sectors[i].ambientLight = 0;
		    sectors[i].extraLight = 0;
		    sectors[i].sound = NULL;
		    sectors[i].soundVolume = 0;

		    while(1)
		    {
			    line = parser.getLine( error );

			    text = line.getString( error );
			    if( text == "EXTRA" )
			    {
				    line.getString( error );
				    sectors[i].extraLight = line.getFloat( error );
			    }
			    if( text == "AMBIENT" ) 
			    {
				    line.getString( error );
				    sectors[i].ambientLight = line.getFloat( error );
			    }
    			
			    if( text == "COLORMAP" )
			    {
				    colormap = line.getInt( error );
			    }

			    if( text == "SOUND" )
			    {
				    text = line.getString( error );
				    sectors[i].sound = sounds[text];
				    sectors[i].soundVolume = line.getFloat( error );
			    }

			    if( text == "VERTICES" )
			    {
				    break;
			    }
			    if( text == "TINT" )
			    {
				    sectors[i].tint.x = line.getFloat( error );
				    sectors[i].tint.y = line.getFloat( error );
				    sectors[i].tint.z = line.getFloat( error );
			    }
		    }

		    line.matchString( "VERTICES", error );
		    numVerts = line.getInt( error );

		    for( j = 0 ; j < numVerts ; j++ )
		    {
			    line = parser.getLine( error );
		    }

		    line = parser.getLine( error );

		    line.matchString( "SURFACES", error );
		    start = line.getInt( error );
		    sectors[i].drawing = 0;
		    sectors[i].num = i;
		    sectors[i].numSurfaces = line.getInt( error );
            sectors[i].surfaces = new World::Surface*[sectors[i].numSurfaces];

		    for( j = 0 ; j < sectors[i].numSurfaces ; j++ )
		    {
			    sectors[i].surfaces[j] = &surfaces[start + j];
			    if( sectors[i].surfaces[j]->adjoined )
			    {
				    adjoins[sectors[i].surfaces[j]->adjnum].sector = &sectors[i];
				    adjoins[sectors[i].surfaces[j]->adjnum].surface = sectors[i].surfaces[j];
			    }
    /*			if(sectors[i].surfaces[j]->polygon.GetTexture()!=NULL)
			    {
				    index=sectors[i].surfaces[j]->polygon.GetTexture()->num;
				    if(textures[index].data==NULL)
					    JK_Level_LoadMAT(textureFilenames[index], &textures[index], &colormaps[colormap]);
			    }*/
		    }
	    }

	    for( i = 0 ; i < numSurfaces ; i++ )
	    if( surfaces[i].adjoined )
	    {
		    surfaces[i].adjoin = adjoins[adjoins[surfaces[i].adjnum].mirror].sector;
		    surfaces[i].mirror = adjoins[adjoins[surfaces[i].adjnum].mirror].surface;
	    }
	    else 
	    {
		    surfaces[i].adjoin = NULL;
		    surfaces[i].mirror = NULL;
	    }

    /*	for(i=0;i<numTextures;i++)
	    {
		    if(!textures[i].valid)
			    JK_Level_LoadMAT(textureFilenames[i], &textures[i], &colormaps[0]);
	    }*/

	    // =====================================================================

	    parser.findString( "Section: MODELS", error );

	    line = parser.getLine( error );
	    line.matchString( "World models", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getFloat( error );

		    filename = line.getString( error );
		    models.push_back( new Render::Model( filename ), filename );
	    }

	    // =====================================================================

	    parser.findString( "Section: SPRITES", error );

	    line = parser.getLine( error );
	    line.matchString( "World sprites", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getInt( error );

		    filename = line.getString( error );
		    sprites.push_back( new Render::Sprite( filename ), filename );
	    }

	    // =====================================================================

	    parser.findString( "Section: KEYFRAMES", error );

	    line = parser.getLine( error );
	    line.matchString( "World keyframes", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getFloat( error );

		    filename = line.getString( error );
            keyframes.push_back( new Jk::Key( filename ), filename );
	    }

    // =====================================================================

	    parser.findString( "Section: ANIMCLASS", error );

	    line = parser.getLine( error );
	    line.matchString( "World puppets", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getInt( error );

		    filename = line.getString( error );
            animClasses.push_back( new Jk::AnimClass( filename ), filename );
	    }

    // =====================================================================

	    parser.findString( "Section: Soundclass", error );

	    line = parser.getLine( error );
	    line.matchString( "World soundclasses", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getInt( error );

		    filename = line.getString( error );
		    soundClasses.push_back( new Sound::Class( filename ), filename );
	    }

    // =====================================================================

	    parser.findString( "Section: cogscripts", error );

	    line = parser.getLine( error );
	    line.matchString( "World scripts", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getInt( error );

		    filename = line.getString( error );
            cogScripts.push_back( new Cog::Script( filename ), filename );
	    }

	    // =====================================================================

	    parser.findString( "Section: cogs", error );

	    line = parser.getLine( error );
	    line.matchString( "World cogs", error );
	    numEntries = line.getInt( error );
    	
	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getInt( error );

		    filename = line.getString( error );
            cogs.push_back( new Cog::Script( *cogScripts[filename] ) );

		    cogs[i]->SaveArgumentString( line.rest() );
	    }

    // =====================================================================

	    parser.findString( "Section: TEMPLATES", error );

	    line = parser.getLine( error );
	    line.matchString( "World templates", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
		    text = line.getString( error );
		    text2 = line.getString( error );
    	
            if( text2 == "none" ) templates.push_back( new Jk::Template( text ), text );
            else				 templates.push_back( new Jk::Template( text, templates[text2] ), text );

		    while(1)
		    {
			    text = line.getString( error );
			    if( error ) break;

			    templates[i]->AddParam( text );
		    }
	    }


    // =====================================================================

	    parser.findString( "Section: Things", error );

	    line = parser.getLine( error );
	    line.matchString( "World things", error );
	    numEntries = line.getInt( error );

	    for( i = 0 ; i < numEntries ; i++ )
	    {
		    line = parser.getLine( error );
    		
		    line.matchString( "end", error );
		    if( !error )
		    {
			    break;
		    }

		    line.getFloat( error );

		    text = line.getString( error );

		    text = line.getString( error );

		    position.x = line.getFloat( error );
		    position.y = line.getFloat( error );
		    position.z = line.getFloat( error );

		    rotation.x = line.getFloat( error );
		    rotation.y = line.getFloat( error );
		    rotation.z = line.getFloat( error );

		    index = line.getInt( error );

		    thing = shared_ptr<World::Thing>( new World::Thing( templates[text], position, rotation, &sectors[index] ) );
		    thing->num = things.size();
		    things.push_back( thing );
    		
		    if( text == "walkplayer" && !foundWalkplayer )
		    {		
			    player = things[i];
			    foundWalkplayer = true;
		    }
    		
		    sectors[index].AddThing( things[i].get() );

		    while( 1 )
		    { 
			    text = line.getString( error );
			    if( error ) break;

			    things[i]->GetTemplate()->AddParam( text );
		    }
		    things[i]->ProcessTemplate();

            
	    }

	    for( i = 0 ; i < cogs.size() ; i++ )
	    {
		    if( cogs[i] )
		    {
			    cogs[i]->ParseArgumentString();
			    cogs[i]->LinkParameters();
		    }
	    }
    }
}
