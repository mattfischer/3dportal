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

template class U_Collection<C_Script>;

struct Adjoin {
	int mirror;
	int flags;
	W_Sector *sector;
	W_Surface *surface;
};

extern shared_ptr<W_Thing> player;

JK_Level currentLevel;

char **textureFilenames;

void JK_Level_Load( const string& name )
{
	HANDLE levelFile;
	int size;
	string data;
	char *dataPointer;
	DWORD dummy;
	int pos, pos2;
	string line;
	string text;
	string text2;
	int error;
	int i, j;
	string filename;
	int index;
	int numVerts;
	float x,y,z;
	int colormap;
	int start;
	int found;
	int numEntries;
	M_Vector position, rotation;
	vector<string> textureNames;

	W_Thing *newThing;
	shared_ptr<W_Thing> thing;

	int numVertices;
	M_Vector *vertices;
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
	
	levelFile = CreateFile( name.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	if( levelFile == INVALID_HANDLE_VALUE )
	{
		fullname = "jkl\\" + name;
		found = JK_GOB_GetFile( fullname, &dataPointer, &size );
	}
	else
	{
		size = GetFileSize( levelFile, NULL );
		dataPointer = new char[size];
		ReadFile( levelFile, dataPointer, size, &dummy, NULL );
		CloseHandle( levelFile );
	}
	data = dataPointer;
	delete[] dataPointer;

	pos = 0;

	// =====================================================================

	JKP_FindString( data, pos, size, "SECTION: HEADER", error );
	
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "World Gravity", error );
	currentLevel.gravity = JKP_GetFloat( line, pos2, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "Ceiling Sky Z", error );
	currentLevel.ceilingSkyZ = JKP_GetFloat( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "Horizon Distance", error );
	currentLevel.horizonDistance = JKP_GetFloat( line, pos2, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "Horizon Pixels per Rev", error );
	currentLevel.numPixelsPerRev = JKP_GetFloat( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "Horizon Sky Offset", error );
	currentLevel.horizonOffsetX = JKP_GetFloat( line, pos2, error );
	currentLevel.horizonOffsetY = JKP_GetFloat( line, pos2, error );

	line = JKP_GetNonEmptyLine( data, pos, size, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "LOD Distances", error );
	currentLevel.LODDistances[0] = JKP_GetFloat( line, pos2, error );
	currentLevel.LODDistances[1] = JKP_GetFloat( line, pos2, error );
	currentLevel.LODDistances[2] = JKP_GetFloat( line, pos2, error );
	currentLevel.LODDistances[3] = JKP_GetFloat( line, pos2, error );

	// =====================================================================

	JKP_FindString( data, pos, size, "SECTION: SOUNDS", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World sounds", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}

		error = 0;
		
		filename = JKP_GetString( line, pos2, error );
		currentLevel.sounds.push_back( new S_Sound( filename ), filename );
	}

	// =====================================================================

	JKP_FindString( data, pos, size, "SECTION: MATERIALS", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World materials", error );
	numEntries = JKP_GetInt( line, pos2, error );
	textureNames.resize( numEntries );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}

		error = 0;
		JKP_GetFloat( line, pos2, error );

		filename = U_Lowercase( JKP_GetString( line, pos2, error ) );
		
		currentLevel.textures.push_back( new R_Texture( filename ), filename );
		textureNames[i] = filename;
	}

	// =====================================================================

	JKP_FindString( data, pos, size, "SECTION: GEORESOURCE", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World Colormaps", error );
	numEntries = JKP_GetInt( line, pos2, error );
//	currentLevel.colormaps.Allocate( numEntries );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_GetFloat( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.colormaps.push_back( new JK_Colormap( filename ), filename );
	}

	// -------------------------------------------------------------------------

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World vertices", error );
	numVertices = JKP_GetInt( line, pos2, error );
	vertices = new M_Vector[numVertices];

	for( i = 0 ; i < numVertices ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_GetFloat( line, pos2, error );

		vertices[i].x = JKP_GetFloat( line, pos2, error );
		vertices[i].y = JKP_GetFloat( line, pos2, error );
		vertices[i].z = JKP_GetFloat( line, pos2, error );
	}

	
	// -------------------------------------------------------------------------
	
	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World texture vertices", error );
	numTextureVertices = JKP_GetInt( line, pos2, error );
	textureVertices = new R_TextureVertex[numTextureVertices];

	for( i = 0 ; i < numTextureVertices ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_GetFloat( line, pos2, error );

		textureVertices[i].u = JKP_GetFloat( line, pos2, error );
		textureVertices[i].v = JKP_GetFloat( line, pos2, error );
	}


	// -------------------------------------------------------------------------

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World adjoins", error );
	numAdjoins = JKP_GetInt( line, pos2, error );
	adjoins = new Adjoin[numAdjoins];

	for( i = 0 ; i < numAdjoins ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_GetInt( line, pos2, error );

		adjoins[i].flags = JKP_GetHex( line, pos2, error );
		adjoins[i].mirror = JKP_GetInt( line, pos2, error );
	}


	// -------------------------------------------------------------------------

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World surfaces", error );
	currentLevel.numSurfaces = JKP_GetInt( line, pos2, error );
	currentLevel.surfaces = new W_Surface[currentLevel.numSurfaces];

	for( i = 0 ; i < currentLevel.numSurfaces ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_GetFloat( line, pos2, error );

		index = JKP_GetInt( line, pos2, error );
		currentLevel.surfaces[i].num = i;
		currentLevel.surfaces[i].flags = JKP_GetHex( line, pos2, error );
		currentLevel.surfaces[i].face = JKP_GetHex( line, pos2, error );
		currentLevel.surfaces[i].geo = JKP_GetInt( line, pos2, error );
		JKP_GetFloat( line, pos2, error );
		JKP_GetFloat( line, pos2, error );

		currentLevel.surfaces[i].adjnum = JKP_GetInt( line, pos2, error );
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
		
		JKP_GetFloat( line, pos2, error );

		numVerts = JKP_GetInt( line, pos2, error );
		currentLevel.surfaces[i].polygon = W_Poly( numVerts, 4 );

		if( index != -1 ) currentLevel.surfaces[i].polygon.SetTexture( currentLevel.textures[textureNames[index]] );
		else   		      currentLevel.surfaces[i].polygon.SetTexture( NULL );

		for( j = 0 ; j < numVerts ; j++ )
		{
			index = JKP_GetInt( line, pos2, error );
			currentLevel.surfaces[i].polygon[j].position = vertices[index];
			
			index = JKP_GetInt( line, pos2, error );
			if( currentLevel.surfaces[i].polygon.GetTexture() != NULL ) 
			{
				currentLevel.surfaces[i].polygon[j].texture = textureVertices[index];
				//surfaces[i].polygon[j].texture.u/=surfaces[i].polygon.GetTexture()->sizeX;
				//surfaces[i].polygon[j].texture.v/=surfaces[i].polygon.GetTexture()->sizeY;
			}
		}	

		for( j = 0 ; j < numVerts ; j++ ) currentLevel.surfaces[i].polygon[j].intensity = JKP_GetFloat( line, pos2, error );
	}

	// -------------------------------------------------------------------------


	for( i = 0 ; i < currentLevel.numSurfaces ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;

		JKP_GetFloat( line, pos2, error );
		x = JKP_GetFloat( line, pos2, error );
		y = JKP_GetFloat( line, pos2, error );
		z = JKP_GetFloat( line, pos2, error );
		
		currentLevel.surfaces[i].polygon.SetPlane( M_Vector( x, y, z) );
	}


	// =====================================================================

	JKP_FindString( data, pos, size, "Section: SECTORS", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World sectors", error );
	currentLevel.numSectors = JKP_GetInt( line, pos2, error );
	currentLevel.sectors = new W_Sector[currentLevel.numSectors];
	
	for( i = 0 ; i < currentLevel.numSectors ; i++ )
	{
		currentLevel.sectors[i].ambientLight = 0;
		currentLevel.sectors[i].extraLight = 0;
		currentLevel.sectors[i].sound = NULL;
		currentLevel.sectors[i].soundVolume = 0;

		while(1)
		{
			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
			text = JKP_GetString( line, pos2, error );
			if( text == "EXTRA" )
			{
				JKP_GetString( line, pos2, error );
				currentLevel.sectors[i].extraLight = JKP_GetFloat( line, pos2, error );
			}
			if( text == "AMBIENT" ) 
			{
				JKP_GetString( line, pos2, error );
				currentLevel.sectors[i].ambientLight = JKP_GetFloat( line, pos2, error );
			}
			
			if( text == "COLORMAP" )
			{
				colormap = JKP_GetInt( line, pos2, error );
			}

			if( text == "SOUND" )
			{
				text = JKP_GetString( line, pos2, error );
				currentLevel.sectors[i].sound = currentLevel.sounds[text];
				currentLevel.sectors[i].soundVolume = JKP_GetFloat( line, pos2, error );
			}

			if( text == "VERTICES" )
			{
				break;
			}
			if( text == "TINT" )
			{
				currentLevel.sectors[i].tint.x = JKP_GetFloat( line, pos2, error );
				currentLevel.sectors[i].tint.y = JKP_GetFloat( line, pos2, error );
				currentLevel.sectors[i].tint.z = JKP_GetFloat( line, pos2, error );
			}
		}

		pos2 = 0;

		JKP_MatchString( line, pos2, "VERTICES", error );
		numVerts = JKP_GetInt( line, pos2, error );

		for( j = 0 ; j < numVerts ; j++ )
		{
			line = JKP_GetNonEmptyLine( data, pos, size, error );
		}

		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;

		JKP_MatchString( line, pos2, "SURFACES", error );
		start = JKP_GetInt( line, pos2, error );
		currentLevel.sectors[i].drawing = 0;
		currentLevel.sectors[i].num = i;
		currentLevel.sectors[i].numSurfaces = JKP_GetInt( line, pos2, error );
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

	JKP_FindString( data, pos, size, "Section: MODELS", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World models", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}
		error = 0;
		JKP_GetFloat( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.models.push_back( new R_Model( filename ), filename );
	}

	// =====================================================================

	JKP_FindString( data, pos, size, "Section: SPRITES", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World sprites", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error==0 )
		{
			break;
		}
		error = 0;
		JKP_GetInt( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.sprites.push_back( new R_Sprite( filename ), filename );
	}

	// =====================================================================

	JKP_FindString( data, pos, size, "Section: KEYFRAMES", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World keyframes", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}
		error = 0;
		JKP_GetFloat( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.keyframes.push_back( new JK_Key( filename ), filename );
	}

// =====================================================================

	JKP_FindString( data, pos, size, "Section: ANIMCLASS", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World puppets", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error==0 )
		{
			break;
		}
		error = 0;
		JKP_GetInt( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.animClasses.push_back( new JK_AnimClass( filename ), filename );
	}

// =====================================================================

	JKP_FindString( data, pos, size, "Section: Soundclass", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World soundclasses", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error==0 )
		{
			break;
		}
		error = 0;
		JKP_GetInt( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.soundClasses.push_back( new S_SoundClass( filename ), filename );
	}

// =====================================================================

	JKP_FindString( data, pos, size, "Section: cogscripts", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World scripts", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}
		error = 0;
		JKP_GetInt( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.cogScripts.push_back( new C_Script( filename ), filename );
	}

	// =====================================================================

	JKP_FindString( data, pos, size, "Section: cogs", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World cogs", error );
	numEntries = JKP_GetInt( line, pos2, error );
	
	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}
		error = 0;
		JKP_GetInt( line, pos2, error );

		filename = JKP_GetString( line, pos2, error );
		currentLevel.cogs.push_back( new C_Script( *currentLevel.cogScripts[filename] ) );

		currentLevel.cogs[i]->SaveArgumentString( line.substr( pos2 ) );
	}

// =====================================================================

	JKP_FindString( data, pos, size, "Section: TEMPLATES", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World templates", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		text = JKP_GetString( line, pos2, error );
		text2 = JKP_GetString( line, pos2, error );
	
		if( text2 == "none" ) currentLevel.templates.push_back( new JK_Template( text ), text );
		else				 currentLevel.templates.push_back( new JK_Template( text, currentLevel.templates[text2] ), text );

		while(1)
		{
			text = JKP_GetString( line, pos2, error );
			if( error ) break;

			currentLevel.templates[i]->AddParam( text );
		}
	}


// =====================================================================

	JKP_FindString( data, pos, size, "Section: Things", error );

	pos2 = 0;
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	JKP_MatchString( line, pos2, "World things", error );
	numEntries = JKP_GetInt( line, pos2, error );

	for( i = 0 ; i < numEntries ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		
		JKP_MatchString( line, pos2, "end", error );
		if( error == 0 )
		{
			break;
		}
		error = 0;
		JKP_GetFloat( line, pos2, error );

		text = JKP_GetString( line, pos2, error );

		text = JKP_GetString( line, pos2, error );

		position.x = JKP_GetFloat( line, pos2, error );
		position.y = JKP_GetFloat( line, pos2, error );
		position.z = JKP_GetFloat( line, pos2, error );

		rotation.x = JKP_GetFloat( line, pos2, error );
		rotation.y = JKP_GetFloat( line, pos2, error );
		rotation.z = JKP_GetFloat( line, pos2, error );

		index = JKP_GetInt( line, pos2, error );

		thing = shared_ptr<W_Thing>( new W_Thing( currentLevel.templates[text], position, rotation, &currentLevel.sectors[index] ) );
		thing->SetNum( currentLevel.things.size() );
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
			error = 0;
			text = JKP_GetString( line, pos2, error );
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

