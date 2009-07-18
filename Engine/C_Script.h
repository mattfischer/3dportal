#ifndef C_SCRIPT_H
#define C_SCRIPT_H

#include "C_AST.h"
#include "C_Stack.h"

#include <windows.h>

#include <vector>
#include <string>

using std::vector;
using std::string;

//#define SAFECOGSINTERNAL

namespace Cog
{
    struct Context 
    {
	    Stack stack;
	    int sender;
	    int senderType;
	    int source;
	    int senderId;
	    bool touched;
    };


    struct Symbol
    {
	    SymbolType type;
	    string name;
	    void *data;
	    bool local;
	    bool noLink;
	    int mask;
	    int linkId;
    };

    struct Message
    {
	    string name;
	    int statementStart;
    };

    struct StartInfo
    {
	    int statement;
	    int sender;
	    int senderType;
	    int source;
	    float time;
	    bool touched;
    };

    class Script
    {
    public:
	    Script( const string& filename );
	    Script( Script &c );

	    void SaveArgumentString( const string& s );
	    void ParseArgumentString();
	    void Message( const string& message, int sender, int senderType, int source, bool synchronous = false );

	    void LinkParameters();
    	
	    void StartTimer( float t );

	    static void LoadSafeCogs();
	    static void Setup();
    protected:
    	
	    static HANDLE semaphore;

	    vector<Symbol> symbols;

        vector<Cog::Message> messages;
    	
	    ASTNode *code;

	    StartInfo startInfo;
    	
	    int flags;

	    bool safe;

	    bool doingTouched;

	    string argumentString;

    #ifdef SAFECOGSINTERNAL
	    static char *safeCogs[];
    #else
	    static char **safeCogs;
    #endif
	    static int numSafeCogs;
    	
	    // C_Script.cpp
	    void SetupSymbolTable( ASTNode *symbolsNode );
	    void SetupMessages();
	    void Hack( string& source );
    	
	    // C_Execute.cpp
	    void ExecuteStatements( int start, Context &c );

	    bool ExecuteStatement( ASTNode *node, Context &c );
	    void ExecuteAssign( ASTNode *node, Context &c );
	    void ExecuteExpression( ASTNode *node, Context &c );

	    int TypeSize( SymbolType type );
	    SymbolType BaseType( SymbolType type );

	    void Cast( SymbolType source, SymbolType dest, Context &c );
	    SymbolType Promote( SymbolType xType, SymbolType yType, Context &c );

	    void Timer( float t );

	    static void ExecuteKickstart( void *object );
	    static void TimerKickstart( void *object );

	    // C_Script.cpp
	    Math::Vector ParseVector( const string& s );

	    // C_Verb.cpp
	    void ExecuteVerbCall( ASTNode *node, Context &c, bool expression );
    };
}

#endif