#ifndef C_SCRIPT_H
#define C_SCRIPT_H

#include "C_AST.h"
#include "C_Stack.h"

#include "U_Collection.h"

#include <windows.h>

#include <vector>

using std::vector;

//#define SAFECOGSINTERNAL

struct C_Context 
{
	C_Stack stack;
	int sender;
	int senderType;
	int source;
	int senderId;
	bool touched;
};


struct C_Symbol
{
	C_SymbolType type;
	string name;
	void *data;
	bool local;
	bool noLink;
	int mask;
	int linkId;
};

struct C_Message
{
	string name;
	int statementStart;
};

struct C_StartInfo
{
	int statement;
	int sender;
	int senderType;
	int source;
	float time;
	bool touched;
};

class C_Script : public U_NameableItem
{
public:
	C_Script( const string& filename );
	C_Script( C_Script &c );

	void SaveArgumentString( const string& s );
	void ParseArgumentString();
	void Message( const string& message, int sender, int senderType, int source, bool synchronous = false );

	void LinkParameters();
	
	void StartTimer( float t );

	static void LoadSafeCogs();
	static void Setup();
protected:
	
	static HANDLE semaphore;

	vector<C_Symbol> symbols;

	vector<C_Message> messages;
	
	C_ASTNode *code;

	C_StartInfo startInfo;
	
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
	void SetupSymbolTable( C_ASTNode *symbolsNode );
	void SetupMessages();
	void Hack( string& source );
	
	// C_Execute.cpp
	void ExecuteStatements( int start, C_Context &c );

	bool ExecuteStatement( C_ASTNode *node, C_Context &c );
	void ExecuteAssign( C_ASTNode *node, C_Context &c );
	void ExecuteExpression( C_ASTNode *node, C_Context &c );

	int TypeSize( C_SymbolType type );
	C_SymbolType BaseType( C_SymbolType type );

	void Cast( C_SymbolType source, C_SymbolType dest, C_Context &c );
	C_SymbolType Promote( C_SymbolType xType, C_SymbolType yType, C_Context &c );

	void Timer( float t );

	static void ExecuteKickstart( void *object );
	static void TimerKickstart( void *object );

	// C_Script.cpp
	M_Vector ParseVector( const string& s );

	// C_Verb.cpp
	void ExecuteVerbCall( C_ASTNode *node, C_Context &c, bool expression );
};

#endif