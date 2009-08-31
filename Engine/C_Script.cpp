#include "C_Script.h"

#include "C_Tokenizer.h"
#include "C_Parse.h"
#include "C_AST.h"
#include "C_Stack.h"

#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "M_Vector.h"

#include "JK_Key.h"
#include "JK_Level.h"
#include "JK_Template.h"
#include "JK_GOB.h"

#include "R_Model.h"

#include "S_Sound.h"

#include "G_Console.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>

#include "U_Lowercase.h"

namespace Cog
{
    #ifndef SAFECOGSINTERNAL
    char **Script::safeCogs;
    #endif
    int Script::numSafeCogs;

    HANDLE Script::semaphore;

    void Script::Setup()
    {
	    LoadSafeCogs();
    	
	    semaphore=CreateSemaphore(NULL, 1, 1, NULL);
    }

    void Script::LoadSafeCogs()
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

    Script::Script( const string& filename )
    {
	    string buffer;
	    int i;
	    string fullname;
	    string data;

	    ASTNode *root;

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
            data = Jk::Gob::getFile( fullname );
		    Hack( data );

		    Tokenizer t( data.c_str() );

		    root = ParseString( t, filename );
		    if( root->nodeType == NODE_COG_FLAGS )
			    flags = root->lexData.intVal;
		    else
			    flags = 0;

		    code = root->children[1];

		    SetupSymbolTable( root->children[0] );
		    SetupMessages();
		    }

	    //wsprintf(buffer, "Compiled COG file %s, %i symbols, %i messages", filename, numSymbols, numMessages);
	    //CON_Out(buffer);
    }

    void Script::LinkParameters()
    {
	    int i;
	    int index;

	    if( !safe ) return;

	    for( i = 0 ; i < symbols.size() ; i++ )
	    {
		    if( symbols[i].local ) continue;
		    if( symbols[i].noLink ) continue;

		    index = symbols[i].data.intVal;
		    if( index == -1 ) continue;
		    switch( symbols[i].type )
		    {
		    case TYPE_THING:
			    currentLevel.things[index]->AddCogLink( this );
			    break;
		    case TYPE_SURFACE:
			    currentLevel.surfaces[index].AddCogLink( this );
			    break;
		    case TYPE_SECTOR:
			    currentLevel.sectors[index].AddCogLink( this );
			    break;
		    }
	    }
    }

    Script::Script( Script &c )
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
		    symbols.push_back( Symbol() );
		    symbols[i] = c.symbols[i];
		    if( symbols[i].type == TYPE_MESSAGE ) continue;
            symbols[i].data = c.symbols[i].data;
	    }

	    messages = c.messages;
	    code = c.code;
	    safe = c.safe;
	    doingTouched = false;
    }

    void Script::SaveArgumentString( const string& s )
    {
	    argumentString = s;
    }

    void Script::ParseArgumentString()
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
		    if( symbols[symbol].type == TYPE_MESSAGE ) continue;

		    j = i;
		    while( j < totalLength && s[j] != ' ' && s[j] != '\t' && s[j] != '\0' ) j++;

		    switch( symbols[symbol].type )
		    {
		    case TYPE_FLEX: 
		    case TYPE_FLOAT:
			    symbols[symbol].data.floatVal = atof(s.substr( i, j - i ).c_str() );
			    break;
    		
		    case TYPE_AI: 
		    case TYPE_COG: 
		    case TYPE_KEYFRAME: 
		    case TYPE_MATERIAL:
		    case TYPE_MODEL: 
			    symbols[symbol].data.intVal = 0;
			    break;

		    case TYPE_TEMPLATE:
			    symbols[symbol].data.intVal = currentLevel.templates.index( s.substr( i, j - i ) );
			    break;

		    case TYPE_SOUND: 
			    symbols[symbol].data.intVal = currentLevel.sounds.index( s.substr( i, j - i ) );
			    break;

		    case TYPE_INT: 
		    case TYPE_SECTOR: 
		    case TYPE_SURFACE:
		    case TYPE_THING: 
			    symbols[symbol].data.intVal = atoi( s.substr( i, j - i ).c_str() ); 
			    break;

		    case TYPE_VECTOR: 
                symbols[symbol].data.vectorVal = new Math::Vector( ParseVector( s.substr( i, j - i ) ) );
			    break;
		    }
		    i = j+1;
	    }
    }

    void Script::SetupSymbolTable( ASTNode *symbolsNode )
    {
	    int i, j;
	    ASTNode *node;
	    string name;
	    int x;

	    for( i = 0 ; i < symbolsNode->numChildren ; i++ )
	    {
		    symbols.push_back( Symbol() );	
		    node = symbolsNode->children[i];
		    name = node->lexData.stringVal;

		    if( name == "ai" )       symbols[i].type = TYPE_AI; 
		    if( name == "cog" )      symbols[i].type = TYPE_COG;
		    if( name == "flex" )     symbols[i].type = TYPE_FLEX;
		    if( name == "float" )    symbols[i].type = TYPE_FLOAT;
		    if( name == "int" )      symbols[i].type = TYPE_INT;
		    if( name == "keyframe" ) symbols[i].type = TYPE_KEYFRAME;
		    if( name == "material" ) symbols[i].type = TYPE_MATERIAL;
		    if( name == "message" )  symbols[i].type = TYPE_MESSAGE;
		    if( name == "model" )    symbols[i].type = TYPE_MODEL;
		    if( name == "sector" )   symbols[i].type = TYPE_SECTOR;
		    if( name == "sound" )    symbols[i].type = TYPE_SOUND;
		    if( name == "surface" )  symbols[i].type = TYPE_SURFACE;
		    if( name == "template" ) symbols[i].type = TYPE_TEMPLATE;
		    if( name == "thing" )    symbols[i].type = TYPE_THING; 
		    if( name == "vector" )   symbols[i].type = TYPE_VECTOR;

		    symbols[i].name = node->children[0]->lexData.stringVal;

		    switch( symbols[i].type )
		    {
		    case TYPE_FLEX: 
		    case TYPE_FLOAT:
			    symbols[i].data.floatVal = 0;
			    break;
    		
		    case TYPE_AI: 
		    case TYPE_COG: 
		    case TYPE_INT: 
		    case TYPE_KEYFRAME: 
		    case TYPE_MATERIAL:
		    case TYPE_MODEL: 
		    case TYPE_SECTOR: 
		    case TYPE_SOUND: 
		    case TYPE_SURFACE:
		    case TYPE_TEMPLATE:
		    case TYPE_THING: 
		    case TYPE_MESSAGE:
			    symbols[i].data.intVal = 0;
			    break;

		    case TYPE_VECTOR: 
                symbols[i].data.vectorVal = new Math::Vector( 0, 0, 0 );
			    break;
		    }
    		
		    symbols[i].local = false;
		    symbols[i].noLink = false;
		    symbols[i].linkId = -1;
		    symbols[i].mask = 0;

		    if( node->nodeType == NODE_SYMBOL_ENTRY_USE )
		    {
			    for( j = 0 ; j < node->children[1]->numChildren ; j++ )
			    {
				    switch( node->children[1]->children[j]->nodeType )
				    {
				    case NODE_SYMBOL_USE_LOCAL: symbols[i].local = true; break;
				    case NODE_SYMBOL_USE_NOLINK: symbols[i].noLink = true; break;
				    case NODE_SYMBOL_USE_LINKID: symbols[i].linkId = node->children[1]->children[j]->children[0]->lexData.intVal; break;
				    case NODE_SYMBOL_USE_MASK: symbols[i].mask = node->children[1]->children[j]->children[0]->lexData.intVal; break;
				    }
			    }
		    }

		    node = node->children[0];
		    if( node->nodeType == NODE_NAME_INIT )
		    {
			    switch( symbols[i].type )
			    {
			    case TYPE_FLEX: 
			    case TYPE_FLOAT:
				    if( node->children[0]->nodeType == NODE_INT )
				    {
					    symbols[i].data.floatVal = node->children[0]->lexData.intVal;
				    }
				    else if( node->children[0]->nodeType == NODE_FLOAT )
                    {
                        symbols[i].data.floatVal = node->children[0]->lexData.floatVal;
                    }
				    break;
			    case TYPE_INT: 
			    case TYPE_SECTOR: 
			    case TYPE_SURFACE:
			    case TYPE_THING: 
			    case TYPE_MESSAGE:
                    symbols[i].data.intVal = node->children[0]->lexData.intVal;
				    break;
			    case TYPE_AI: 
			    case TYPE_COG: 
                case TYPE_MATERIAL:
				    break;

			    case TYPE_KEYFRAME: 
                    {
                        string filename = Util::Lowercase( node->children[0]->lexData.stringVal );
                        int index = currentLevel.keyframes.index( filename );
                        if( index == -1 )
                        {
                            currentLevel.keyframes.push_back( new Jk::Key( filename ), filename );
                            index = currentLevel.keyframes.index( filename );
                        }
				        symbols[i].data.intVal = index;
                    }
                    break;

			    case TYPE_MODEL: 
                    {
                        string filename = Util::Lowercase( node->children[0]->lexData.stringVal );
                        int index = currentLevel.models.index( filename );
                        if( index == -1 )
                        {
                            currentLevel.models.push_back( new Render::Model( filename ), filename );
                            index = currentLevel.models.index( filename );
                        }
				        symbols[i].data.intVal = index;
                    }
				    break;

			    case TYPE_SOUND: 
                    {
                        string filename = Util::Lowercase( node->children[0]->lexData.stringVal );
                        int index = currentLevel.sounds.index( filename );
                        if( index == -1 )
                        {
                            currentLevel.sounds.push_back( new Sound::Buffer( filename ), filename );
                            index = currentLevel.sounds.index( filename );
                        }
				        symbols[i].data.intVal = index;
                    }
				    break;

			    case TYPE_TEMPLATE:
				    symbols[i].data.intVal = currentLevel.templates.index( node->children[0]->lexData.stringVal );
				    break;

			    case TYPE_VECTOR: 
				    break;
			    }
		    }
	    }
    }

    void Script::SetupMessages()
    {
	    int i;
	    ASTNode *node;
        Cog::Message newMessage;

	    for( i = 0 ; i < code->numChildren ; i++ )
	    {
		    node = code->children[i];
		    if( node->nodeType != NODE_STATEMENT_LABEL ) continue;

		    newMessage.name = node->lexData.stringVal;
		    newMessage.statementStart = i;
		    messages.push_back( newMessage );
	    }
    }

    void Script::Hack( string& source )
    {
	    int i;
	    string buffer;

	    for( i = 0 ; i < source.length() - 6 ; i++ )
	    {
		    buffer = source.substr( i, 5 );
		    if( buffer == "desc_" ) source[i+4] = '=';
	    }
    }

    void Script::Message( const string& message, int sender, int senderType, int source, bool synchronous )
    {
	    int i;
	    string buffer;
	    bool activatedHack;
	    HANDLE newThread;
	    //wsprintf(buffer, "%s: Message %s (safe=%s)", filename, message, safe?"true":"false");
	    //CON_Out(buffer);

	    if( !safe ) return;
    	
	    if( source != -1 && source != currentLevel.player->GetNum() ) return;

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

    void Script::StartTimer(float t)
    {
	    WaitForSingleObject(semaphore, INFINITE);
	    startInfo.time=t;
	    _beginthread(TimerKickstart, 0, this);
    }

    void Script::Timer( float t )
    {
	    int i;
	    Context c;
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

    void Script::TimerKickstart(void *object)
    {
	    ((Script*)object)->Timer(((Script*)object)->startInfo.time);
    }
    	
    Math::Vector Script::ParseVector( const string& s )
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
	    return Math::Vector( pieces[0], pieces[1], pieces[2] );
    }
}