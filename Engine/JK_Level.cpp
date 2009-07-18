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

struct Adjoin {
	int mirror;
	int flags;
	W_Sector *sector;
	W_Surface *surface;
};

extern shared_ptr<W_Thing> player;

Jk::Level currentLevel;

char **textureFilenames;

void JK_Level_Load(const string& name)
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

	W_Thing *newThing;
	shared_ptr<W_Thing> thing;

	int numVertices;
	Math::Vector *vertices;
	int numTextureVertices;
	R_TextureVertex *textureVertices;
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
	currentLevel.gravity = line.getFloat( error );
	
	line = parser.getLine( error );
	line.matchString( "Ceiling Sky Z", error );
	currentLevel.ceilingSkyZ = line.getFloat( error );

	line = parser.getLine( error );
	line.matchString( "Horizon Distance", error );
	currentLevel.horizonDistance = line.getFloat( error );
	
	line = parser.getLine( error );
	line.matchString( "Horizon Pixels per Rev", error );
	currentLevel.numPixelsPerRev = line.getFloat( error );

	line = parser.getLine( error );
	line.matchString( "Horizon Sky Offset", error );
	currentLevel.horizonOffsetX = line.getFloat( error );
	currentLevel.horizonOffsetY = line.getFloat( error );

	line = parser.getLine( error );
	line = parser.getLine( error );

	line = parser.getLine( error );
	line.matchString( "LOD Distances", error );
	currentLevel.LODDistances[0] = line.getFloat( error );
	currentLevel.LODDistances[1] = line.getFloat( error );
	currentLevel.LODDistances[2] = line.getFloat( error );
	currentLevel.LODDistances[3] = line.getFloat( error );

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
		currentLevel.sounds.push_back( new Sound::Buffer( filename ), filename );
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
		
		currentLevel.textures.push_back( new R_Texture( filename ), filename );
		textureNames[i] = filename;
	}

	// =====================================================================

	parser.findString( "SECTION: GEORESOURCE", error );

	line = parser.getLine( error );
	line.matchString( "World Colormaps", error );
	numEntries = line.getInt( error );
//	currentLevel.colormaps.Allocate( numEntries );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = parser.getLine( error );
		
		line.getFloat( error );

		filename = line.getString( error );
		currentLevel.colormaps.push_back( new JK_Colormap( filename ), filename );
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
	textureVertices = new R_TextureVertex[numTextureVertices];

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
	currentLevel.numSurfaces = line.getInt( error );
	currentLevel.surfaces = new W_Surface[currentLevel.numSurfaces];

	for( i = 0 ; i < currentLevel.numSurfaces ; i++ )
	{
		line = parser.getLine( error );
		
		line.getFloat( error );

		index = line.getInt( error );
		currentLevel.surfaces[i].num = i;
		currentLevel.surfaces[i].flags = line.getHex( error );
		currentLevel.surfaces[i].face = line.getHex( error );
		currentLevel.surfaces[i].geo = line.getInt( error );
		line.getFloat( error );
		line.getFloat( error );

		currentLevel.surfaces[i].adjnum = line.getInt( error );
		if( currentLevel.surfaces[i].adjnum != -1 )
		{
			currentLevel.surfaces[i].adjoined = 1;
			currentLevel.surfaces[i].adjoinFlags = adjoins[currentLevel.surfaces[i].adjnum].flags;
		}
		else
		{
			currentLevel.surfaces[i].adjoined = 0;
			currentLevel.surfaces[i].adjoinFlags = 0;
		}
		
		line.getFloat( error );

		numVerts = line.getInt( error );
		currentLevel.surfaces[i].polygon = W_Poly( numVerts, 4 );

		if( index != -1 ) currentLevel.surfaces[i].polygon.SetTexture( currentLevel.textures[textureNames[index]] );
		else   		      currentLevel.surfaces[i].polygon.SetTexture( NULL );

		for( j = 0 ; j < numVerts ; j++ )
		{
			index = line.getInt( error );
			currentLevel.surfaces[i].polygon[j].position = vertices[index];
			
			index = line.getInt( error );
			if( currentLevel.surfaces[i].polygon.GetTexture() != NULL ) 
			{
				currentLevel.surfaces[i].polygon[j].texture = textureVertices[index];
				//surfaces[i].polygon[j].texture.u/=surfaces[i].polygon.GetTexture()->sizeX;
				//surfaces[i].polygon[j].texture.v/=surfaces[i].polygon.GetTexture()->sizeY;
			}
		}	

		for( j = 0 ; j < numVerts ; j++ ) currentLevel.surfaces[i].polygon[j].intensity = line.getFloat( error );
	}

	// -------------------------------------------------------------------------


	for( i = 0 ; i < currentLevel.numSurfaces ; i++ )
	{
		line = parser.getLine( error );

		line.getFloat( error );
		x = line.getFloat( error );
		y = line.getFloat( error );
		z = line.getFloat( error );
		
		currentLevel.surfaces[i].polygon.SetPlane( Math::Vector( x, y, z) );
	}


	// =====================================================================

	parser.findString( "Section: SECTORS", error );

	line = parser.getLine( error );
	line.matchString( "World sectors", error );
	currentLevel.numSectors = line.getInt( error );
	currentLevel.sectors = new W_Sector[currentLevel.numSectors];
	
	for( i = 0 ; i < currentLevel.numSectors ; i++ )
	{
		currentLevel.sectors[i].ambientLight = 0;
		currentLevel.sectors[i].extraLight = 0;
		currentLevel.sectors[i].sound = NULL;
		currentLevel.sectors[i].soundVolume = 0;

		while(1)
		{
			line = parser.getLine( error );

			text = line.getString( error );
			if( text == "EXTRA" )
			{
				line.getString( error );
				currentLevel.sectors[i].extraLight = line.getFloat( error );
			}
			if( text == "AMBIENT" ) 
			{
				line.getString( error );
				currentLevel.sectors[i].ambientLight = line.getFloat( error );
			}
			
			if( text == "COLORMAP" )
			{
				colormap = line.getInt( error );
			}

			if( text == "SOUND" )
			{
				text = line.getString( error );
				currentLevel.sectors[i].sound = currentLevel.sounds[text];
				currentLevel.sectors[i].soundVolume = line.getFloat( error );
			}

			if( text == "VERTICES" )
			{
				break;
			}
			if( text == "TINT" )
			{
				currentLevel.sectors[i].tint.x = line.getFloat( error );
				currentLevel.sectors[i].tint.y = line.getFloat( error );
				currentLevel.sectors[i].tint.z = line.getFloat( error );
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
		currentLevel.sectors[i].drawing = 0;
		currentLevel.sectors[i].num = i;
		currentLevel.sectors[i].numSurfaces = line.getInt( error );
		currentLevel.sectors[i].surfaces = new W_Surface*[currentLevel.sectors[i].numSurfaces];

		for( j = 0 ; j < currentLevel.sectors[i].numSurfaces ; j++ )
		{
			currentLevel.sectors[i].surfaces[j] = &currentLevel.surfaces[start + j];
			if( currentLevel.sectors[i].surfaces[j]->adjoined )
			{
				adjoins[currentLevel.sectors[i].surfaces[j]->adjnum].sector = &currentLevel.sectors[i];
				adjoins[currentLevel.sectors[i].surfaces[j]->adjnum].surface = currentLevel.sectors[i].surfaces[j];
			}
/*			if(currentLevel.sectors[i].surfaces[j]->polygon.GetTexture()!=NULL)
			{
				index=currentLevel.sectors[i].surfaces[j]->polygon.GetTexture()->num;
				if(currentLevel.textures[index].data==NULL)
					JK_Level_LoadMAT(textureFilenames[index], &currentLevel.textures[index], &colormaps[colormap]);
			}*/
		}
	}

	for( i = 0 ; i < currentLevel.numSurfaces ; i++ )
	if( currentLevel.surfaces[i].adjoined )
	{
		currentLevel.surfaces[i].adjoin = adjoins[adjoins[currentLevel.surfaces[i].adjnum].mirror].sector;
		currentLevel.surfaces[i].mirror = adjoins[adjoins[currentLevel.surfaces[i].adjnum].mirror].surface;
	}
	else 
	{
		currentLevel.surfaces[i].adjoin = NULL;
		currentLevel.surfaces[i].mirror = NULL;
	}

/*	for(i=0;i<currentLevel.numTextures;i++)
	{
		if(!currentLevel.textures[i].valid)
			JK_Level_LoadMAT(textureFilenames[i], &currentLevel.textures[i], &colormaps[0]);
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
		currentLevel.models.push_back( new R_Model( filename ), filename );
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
		currentLevel.sprites.push_back( new R_Sprite( filename ), filename );
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
        currentLevel.keyframes.push_back( new Jk::Key( filename ), filename );
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
        currentLevel.animClasses.push_back( new Jk::AnimClass( filename ), filename );
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
		currentLevel.soundClasses.push_back( new Sound::Class( filename ), filename );
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
		currentLevel.cogScripts.push_back( new C_Script( filename ), filename );
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
		currentLevel.cogs.push_back( new C_Script( *currentLevel.cogScripts[filename] ) );

		currentLevel.cogs[i]->SaveArgumentString( line.rest() );
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
	
        if( text2 == "none" ) currentLevel.templates.push_back( new Jk::Template( text ), text );
        else				 currentLevel.templates.push_back( new Jk::Template( text, currentLevel.templates[text2] ), text );

		while(1)
		{
			text = line.getString( error );
			if( error ) break;

			currentLevel.templates[i]->AddParam( text );
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

		thing = shared_ptr<W_Thing>( new W_Thing( currentLevel.templates[text], position, rotation, &currentLevel.sectors[index] ) );
		thing->num = currentLevel.things.size();
		currentLevel.things.push_back( thing );
		
		if( text == "walkplayer" && !foundWalkplayer )
		{		
			player = currentLevel.things[i];
			currentLevel.playerNum = i;
			foundWalkplayer = true;
		}
		
		currentLevel.sectors[index].AddThing( currentLevel.things[i].get() );

		while( 1 )
		{ 
			text = line.getString( error );
			if( error ) break;

			currentLevel.things[i]->GetTemplate()->AddParam( text );
		}
		currentLevel.things[i]->ProcessTemplate();

        
	}

	for( i = 0 ; i < currentLevel.cogs.size() ; i++ )
	{
		if( currentLevel.cogs[i] )
		{
			currentLevel.cogs[i]->ParseArgumentString();
			currentLevel.cogs[i]->LinkParameters();
		}
	}
}

