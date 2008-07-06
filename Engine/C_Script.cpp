#include "C_Script.h"

#include "C_Tokenizer.h"
#include "C_Parse.h"
#include "C_AST.h"
#include "C_Stack.h"

#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "M_Vector.h"

#include "JK_Level.h"
#include "JK_Template.h"
#include "JK_GOB.h"

#include "S_Sound.h"

#include "G_Console.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>

#ifndef SAFECOGSINTERNAL
char **C_Script::safeCogs;
#endif
int C_Script::numSafeCogs;
extern shared_ptr<W_Thing> player;

HANDLE C_Script::semaphore;

void C_Script::Setup()
{
	LoadSafeCogs();
	
	semaphore=CreateSemaphore(NULL, 1, 1, NULL);
}

void C_Script::LoadSafeCogs()
{
	numSafeCogs=0;
#ifdef SAFECOGSINTERNAL
	while(safeCogs[numSafeCogs][0]!='\0') numSafeCogs++;
	
#else
	HANDLE file;
	char *data;
	int fileSize;
	int i,j,c;
	DWORD dummy;

	file=CreateFile("safecogs.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	fileSize=GetFileSize(file, NULL);
	
	data=new char[fileSize];
	ReadFile(file, data, fileSize, &dummy, NULL);

	for(i=0;i<fileSize;i++)
	{
		if(data[i]=='\n') numSafeCogs++;
	}

	safeCogs=new char*[numSafeCogs];

	c=0;
	i=0;
	while(i<fileSize)
	{
		j=i;
		while(j<fileSize && data[j]!='\r') j++;
		safeCogs[c]=new char[j-i+1];
		strncpy(safeCogs[c], data+i, j-i);
		safeCogs[c][j-i]='\0';
		i=j+2;
		c++;
	}
	delete[] data;
#endif
}

C_Script::C_Script( const string& filename )
{
	string buffer;
	int i;
	string fullname;
	string data;
	char *dataPointer;
	int size;

	C_ASTNode *root;

	name = filename;

	
	safe = false;

	doingTouched = false;
	for( i = 0 ; i < numSafeCogs ; i++ )
	{
		if( strlen( safeCogs[i] ) == 0 )
			continue;

		buffer = filename.substr( 0, strlen( safeCogs[i] ) );
		if( !strcmpi( buffer.c_str(), safeCogs[i] ) ) 
			safe = true;
	}

	if( safe )
	{
		fullname = "cog\\" + filename;
		JK_GOB_GetFile( fullname, &dataPointer, &size);
		data = dataPointer;
		delete[] dataPointer;
		Hack( data );

		C_Tokenizer t( data.c_str() );

		root = C_ParseString( t, filename );
		if( root->nodeType == C_AST_COG_FLAGS )
			flags = *(int*)root->lexData;
		else
			flags = 0;

		code = root->children[1];

		SetupSymbolTable( root->children[0] );
		SetupMessages();
		}

	//wsprintf(buffer, "Compiled COG file %s, %i symbols, %i messages", filename, numSymbols, numMessages);
	//CON_Out(buffer);
}

void C_Script::LinkParameters()
{
	int i;
	int index;

	if( !safe ) return;

	for( i = 0 ; i < symbols.size() ; i++ )
	{
		if( symbols[i].local ) continue;
		if( symbols[i].noLink ) continue;

		index = *(int*)symbols[i].data;
		if( index == -1 ) continue;
		switch( symbols[i].type )
		{
		case C_TYPE_THING:
			currentLevel.things[index]->AddCogLink( this );
			break;
		case C_TYPE_SURFACE:
			currentLevel.surfaces[index].AddCogLink( this );
			break;
		case C_TYPE_SECTOR:
			currentLevel.sectors[index].AddCogLink( this );
			break;
		}
	}
}

C_Script::C_Script( C_Script &c )
{
	int i;

	if( !c.safe )
	{
		safe = false;
		return;
	}

	flags = c.flags;
	
	for( i = 0 ; i < c.symbols.size() ; i++ )
	{
		symbols.push_back( C_Symbol() );
		symbols[i] = c.symbols[i];
		if( symbols[i].type == C_TYPE_MESSAGE ) continue;
		symbols[i].data = new char[TypeSize( symbols[i].type )];
		memcpy( symbols[i].data, c.symbols[i].data, TypeSize( symbols[i].type ) );
	}

	messages = c.messages;
	code = c.code;
	name = c.name;
	safe = c.safe;
	doingTouched = false;
}

void C_Script::SaveArgumentString( const string& s )
{
	argumentString = s;
}

void C_Script::ParseArgumentString()
{
	int symbol;
	int i,j, n;
	int totalLength;
	string s;

	if(!safe) return;

	s = argumentString;

	totalLength = s.length();
	i = 0;
	while( i < totalLength && ( s[i] == ' ' || s[i] == '\t' ) ) i++;
		
	for( symbol = 0 ; symbol < symbols.size() ; symbol++ )
	{
		if( i >= totalLength ) break;
		if( symbols[symbol].local ) continue;
		if( symbols[symbol].type == C_TYPE_MESSAGE ) continue;

		j = i;
		while( j < totalLength && s[j] != ' ' && s[j] != '\t' && s[j] != '\0' ) j++;

		switch( symbols[symbol].type )
		{
		case C_TYPE_FLEX: 
		case C_TYPE_FLOAT:
			*(float*)symbols[symbol].data = atof(s.substr( i, j - i ).c_str() );
			break;
		
		case C_TYPE_AI: 
		case C_TYPE_COG: 
		case C_TYPE_KEYFRAME: 
		case C_TYPE_MATERIAL:
		case C_TYPE_MODEL: 
			*(int*) symbols[symbol].data = 0;
			break;

		case C_TYPE_TEMPLATE:
			*(int*)symbols[symbol].data = currentLevel.templates.index( s.substr( i, j - i ) );
			break;

		case C_TYPE_SOUND: 
			*(int*)symbols[symbol].data = currentLevel.sounds.index( s.substr( i, j - i ) );
			break;

		case C_TYPE_INT: 
		case C_TYPE_SECTOR: 
		case C_TYPE_SURFACE:
		case C_TYPE_THING: 
			*(int*)symbols[symbol].data = atoi( s.substr( i, j - i ).c_str() ); 
			break;

		case C_TYPE_VECTOR: 
			*(M_Vector*)symbols[symbol].data = ParseVector( s.substr( i, j - i ) );
			break;
		}
		i = j+1;
	}
}

void C_Script::SetupSymbolTable( C_ASTNode *symbolsNode )
{
	int i, j;
	C_ASTNode *node;
	string name;
	int x;

	for( i = 0 ; i < symbolsNode->numChildren ; i++ )
	{
		symbols.push_back( C_Symbol() );	
		node = symbolsNode->children[i];
		name = (char*)node->lexData;

		if( name == "ai" )       symbols[i].type = C_TYPE_AI; 
		if( name == "cog" )      symbols[i].type = C_TYPE_COG;
		if( name == "flex" )     symbols[i].type = C_TYPE_FLEX;
		if( name == "float" )    symbols[i].type = C_TYPE_FLOAT;
		if( name == "int" )      symbols[i].type = C_TYPE_INT;
		if( name == "keyframe" ) symbols[i].type = C_TYPE_KEYFRAME;
		if( name == "material" ) symbols[i].type = C_TYPE_MATERIAL;
		if( name == "message" )  symbols[i].type = C_TYPE_MESSAGE;
		if( name == "model" )    symbols[i].type = C_TYPE_MODEL;
		if( name == "sector" )   symbols[i].type = C_TYPE_SECTOR;
		if( name == "sound" )    symbols[i].type = C_TYPE_SOUND;
		if( name == "surface" )  symbols[i].type = C_TYPE_SURFACE;
		if( name == "template" ) symbols[i].type = C_TYPE_TEMPLATE;
		if( name == "thing" )    symbols[i].type = C_TYPE_THING; 
		if( name == "vector" )   symbols[i].type = C_TYPE_VECTOR;

		symbols[i].name = (char*)node->children[0]->lexData;

		switch( symbols[i].type )
		{
		case C_TYPE_FLEX: 
		case C_TYPE_FLOAT:
			symbols[i].data = new float; 
			*(float*)symbols[i].data = 0;
			break;
		
		case C_TYPE_AI: 
		case C_TYPE_COG: 
		case C_TYPE_INT: 
		case C_TYPE_KEYFRAME: 
		case C_TYPE_MATERIAL:
		case C_TYPE_MODEL: 
		case C_TYPE_SECTOR: 
		case C_TYPE_SOUND: 
		case C_TYPE_SURFACE:
		case C_TYPE_TEMPLATE:
		case C_TYPE_THING: 
		case C_TYPE_MESSAGE:
			symbols[i].data = new int; 
			*(int*)symbols[i].data = 0;
			break;

		case C_TYPE_VECTOR: 
			symbols[i].data = new M_Vector; 
			*(M_Vector*)symbols[i].data = M_Vector( 0, 0, 0 );
			break;
		}
		
		symbols[i].local = false;
		symbols[i].noLink = false;
		symbols[i].linkId = -1;
		symbols[i].mask = 0;

		if( node->nodeType == C_AST_SYMBOL_ENTRY_USE )
		{
			for( j = 0 ; j < node->children[1]->numChildren ; j++ )
			{
				switch( node->children[1]->children[j]->nodeType )
				{
				case C_AST_SYMBOL_USE_LOCAL: symbols[i].local = true; break;
				case C_AST_SYMBOL_USE_NOLINK: symbols[i].noLink = true; break;
				case C_AST_SYMBOL_USE_LINKID: symbols[i].linkId = *(int*)node->children[1]->children[j]->children[0]->lexData; break;
				case C_AST_SYMBOL_USE_MASK: symbols[i].mask = *(int*)node->children[1]->children[j]->children[0]->lexData; break;
				}
			}
		}

		node = node->children[0];
		if( node->nodeType == C_AST_NAME_INIT )
		{
			switch( symbols[i].type )
			{
			case C_TYPE_FLEX: 
			case C_TYPE_FLOAT:
				if( node->children[0]->nodeType == C_AST_INT )
				{
					x = *(int*)node->children[0]->lexData;
					*(float*)symbols[i].data = x;
				}
				else if( node->children[0]->nodeType == C_AST_FLOAT )
					memcpy( symbols[i].data, node->children[0]->lexData, TypeSize( symbols[i].type ) );
				break;
			case C_TYPE_INT: 
			case C_TYPE_SECTOR: 
			case C_TYPE_SURFACE:
			case C_TYPE_THING: 
			case C_TYPE_MESSAGE:
				memcpy( symbols[i].data, node->children[0]->lexData, TypeSize( symbols[i].type ) );
				break;
			case C_TYPE_AI: 
			case C_TYPE_COG: 
			case C_TYPE_KEYFRAME: 
			case C_TYPE_MATERIAL:
				break;

			case C_TYPE_MODEL: 
				*(int*)symbols[i].data = currentLevel.models.index( (char*)node->children[0]->lexData );
				break;

			case C_TYPE_SOUND: 
				*(int*)symbols[i].data = currentLevel.sounds.index( (char*)node->children[0]->lexData );
				break;

			case C_TYPE_TEMPLATE:
				*(int*)symbols[i].data = currentLevel.templates.index( (char*)node->children[0]->lexData );
				break;

			case C_TYPE_VECTOR: 
				break;
			}
		}
	}
}

void C_Script::SetupMessages()
{
	int i;
	C_ASTNode *node;
	C_Message newMessage;

	for( i = 0 ; i < code->numChildren ; i++ )
	{
		node = code->children[i];
		if( node->nodeType != C_AST_STATEMENT_LABEL ) continue;

		newMessage.name = (char*)node->lexData;
		newMessage.statementStart = i;
		messages.push_back( newMessage );
	}
}

void C_Script::Hack( string& source )
{
	int i;
	string buffer;

	for( i = 0 ; i < source.length() - 6 ; i++ )
	{
		buffer = source.substr( i, 5 );
		if( buffer == "desc_" ) source[i+4] = '=';
	}
}

void C_Script::Message( const string& message, int sender, int senderType, int source, bool synchronous )
{
	int i;
	string buffer;
	bool activatedHack;
	HANDLE newThread;
	//wsprintf(buffer, "%s: Message %s (safe=%s)", filename, message, safe?"true":"false");
	//CON_Out(buffer);

	if( !safe ) return;
	
	if( source != -1 && source != player->GetNum() ) return;

	if( message == "activated" ) activatedHack = true;
	else activatedHack = false;

	if( message == "touched" )
	{
		if( doingTouched ) return;
		else doingTouched = true;
	}

	for( i = 0 ; i < messages.size() ; i++ )
		if( messages[i].name == message || ( activatedHack && messages[i].name == "activate" ) )
		{
			WaitForSingleObject( semaphore, INFINITE );
			startInfo.statement = messages[i].statementStart;
			startInfo.sender = sender;
			startInfo.senderType = senderType;
			startInfo.source = source;
			startInfo.touched = doingTouched;
			newThread = (HANDLE)_beginthread( ExecuteKickstart, 0, this );
			if( synchronous )
				WaitForSingleObject( newThread, INFINITE );
		}
}

void C_Script::StartTimer(float t)
{
	WaitForSingleObject(semaphore, INFINITE);
	startInfo.time=t;
	_beginthread(TimerKickstart, 0, this);
}

void C_Script::Timer( float t )
{
	int i;
	C_Context c;
	char buffer[100];

	ReleaseSemaphore( semaphore, 1, NULL );
	Sleep( t * 1000 );
	//wsprintf(buffer, "%s: Message timer", filename);
	//CON_Out(buffer);

	for( i = 0 ; i < messages.size() ; i++ )
		if( messages[i].name == "timer" )
		{
			ExecuteStatements( messages[i].statementStart, c );
			break;
		}
}

void C_Script::TimerKickstart(void *object)
{
	((C_Script*)object)->Timer(((C_Script*)object)->startInfo.time);
}
	
M_Vector C_Script::ParseVector( const string& s )
{
	float pieces[3];
	string newString;
	int i,j,n;

	for( i = 0 ; i < 3 ; i++ ) pieces[i] = 0;

	i = 1;
	for( n = 0 ; n < 3 ; n++ )
	{
		for( j = i ; s[j] != '/' && s[j] != ')' ; j++ ) ;
		
		newString = s.substr( i, j - i );
		pieces[n] = atof( newString.c_str() );
		i = j + 1;
		if( s[j] == ')' ) break;
	}
	return M_Vector( pieces[0], pieces[1], pieces[2] );
}
/*
void C_Script::SetNum(int n)
{

}*/