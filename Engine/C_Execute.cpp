#include "C_Script.h"
#include "C_Stack.h"
#include "M_Vector.h"

#include "G_Console.h"

void C_Script::ExecuteStatements( int start, C_Context &c )
{
	int i;
	bool statementReturn;

	for( i = start ; i < code->numChildren ; i++ )
	{
		statementReturn = ExecuteStatement( code->children[i], c );
		if( statementReturn ) break;
	}

	if( c.touched ) 
		doingTouched = false;
}

void C_Script::ExecuteKickstart( void *object )
{
	C_Context c;
	int statement;
	int i;
	bool found;

	c.sender = ( (C_Script*)object )->startInfo.sender;
	c.senderType=( (C_Script*)object )->startInfo.senderType;
	c.source=( (C_Script*)object )->startInfo.source;
	c.touched=( (C_Script*)object )->startInfo.touched;

	found = false;

	for( i = 0 ; i < ( (C_Script*)object )->symbols.size() ; i++ )
		if( ( (C_Script*)object )->symbols[i].type == c.senderType && *(int*)( (C_Script*)object )->symbols[i].data == c.sender )
		{
			c.senderId = ( (C_Script*)object )->symbols[i].linkId;
			found = true;
			break;
		}
	c.senderId = -1;

	srand( GetCurrentThreadId() * rand() );
	statement = ( (C_Script*)object )->startInfo.statement;
	ReleaseSemaphore( semaphore, 1, NULL );
	( (C_Script*)object )->ExecuteStatements( statement, c );
}

bool C_Script::ExecuteStatement( C_ASTNode *node, C_Context &c )
{
	int i;
	int start;
	bool statementReturn;
	char buffer[100];
	int x;
	int status;

	switch( node->nodeType )
	{
	case C_AST_STATEMENT_LIST: 
		for( i = 0 ; i < node->numChildren ; i++ )
		{
			statementReturn = ExecuteStatement( node->children[i], c );
			if( statementReturn ) return true;
		}
		return false;

	case C_AST_STATEMENT_CALL: 
		for( i = 0 ; i < messages.size() ; i++ )
			if( messages[i].name == (char*)node->lexData )
				ExecuteStatements( messages[i].statementStart, c );
		return false;

	case C_AST_STATEMENT_ASSIGN: 
	case C_AST_STATEMENT_ASSIGN_INDEXED: 
		ExecuteAssign( node, c );
		return false;

	case C_AST_STATEMENT_IF: 
	case C_AST_STATEMENT_IF_ELSE:
		ExecuteExpression( node->children[0], c );
		x = c.stack.PopInt();
		statementReturn = false;
		if( x ) statementReturn = ExecuteStatement( node->children[1], c );
		else if( node->nodeType == C_AST_STATEMENT_IF_ELSE ) 
			statementReturn = ExecuteStatement( node->children[2], c );
		return statementReturn;

	case C_AST_STATEMENT_FOR: 
		ExecuteExpression( node->children[0], c );
		while( 1 )
		{
			ExecuteExpression( node->children[1], c );
			status = c.stack.PopInt();
			if( status == 0 ) break;
			statementReturn = ExecuteStatement( node->children[3], c );
			if( statementReturn ) return true;
			ExecuteExpression( node->children[2], c );
		}
		return false;

	case C_AST_STATEMENT_WHILE: 
		while( 1 )
		{
			ExecuteExpression( node->children[0], c );
			status = c.stack.PopInt();
			if( status == 0 ) break;
			statementReturn = ExecuteStatement( node->children[1], c );
			if( statementReturn ) return true;
		}
		return false;

	case C_AST_STATEMENT_VERBCALL: 
		ExecuteVerbCall( node, c, false );
		return false;

	case C_AST_STATEMENT_RETURN: 
		return true;
	
	case C_AST_STATEMENT_LABEL: 
	case C_AST_STATEMENT_EMPTY: 
		return false;
	}

	return false;
}

void C_Script::ExecuteAssign( C_ASTNode *node, C_Context &c )
{
	int i;
	char buffer[100];
	int index;

	switch( node->nodeType )
	{
	case C_AST_STATEMENT_ASSIGN:
	case C_AST_EXPRESSION_ASSIGN:
		ExecuteExpression( node->children[0], c );
		for( i = 0 ; i < symbols.size() ; i++ )
			if( !strcmpi( symbols[i].name.c_str(), (char*)node->lexData ) )
			{
				Cast( node->children[0]->type, symbols[i].type, c );
				c.stack.Pop( symbols[i].data, TypeSize( symbols[i].type ) );
				break;
			}
		break;
	case C_AST_STATEMENT_ASSIGN_INDEXED:
	case C_AST_EXPRESSION_ASSIGN_INDEXED:
		ExecuteExpression( node->children[1], c );
		ExecuteExpression( node->children[0], c );
		index = c.stack.PopInt();
		for( i = 0 ; i < symbols.size() ; i++ )
			if( symbols[i].name == (char*)node->lexData )
			{
				Cast( node->children[0]->type, symbols[i+index].type, c );
				c.stack.Pop( symbols[i].data, TypeSize( symbols[i+index].type ) );
				break;
			}
		break;
	}
	
}

int C_Script::TypeSize(C_SymbolType type)
{
	switch(type)
	{
		case C_TYPE_FLEX: 
		case C_TYPE_FLOAT:
			return sizeof(float);
		
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
			return sizeof(int);

		case C_TYPE_VECTOR: 
			return sizeof(Math::Vector);
	}
}

C_SymbolType C_Script::BaseType(C_SymbolType type)
{
	switch(type)
	{
		case C_TYPE_FLEX: 
		case C_TYPE_FLOAT:
			return C_TYPE_FLOAT;
		
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
			return C_TYPE_INT;

		case C_TYPE_VECTOR: 
			return C_TYPE_VECTOR;
	}
}


void C_Script::ExecuteExpression( C_ASTNode *node, C_Context &c )
{
	int i;
	int xi,yi, ri;
	float xf, yf, rf;
	Math::Vector xv;
	C_SymbolType type;
	int index;

	switch( node->nodeType )
	{
	case C_AST_EXPRESSION_ASSIGN:
	case C_AST_EXPRESSION_ASSIGN_INDEXED: 
		ExecuteAssign( node, c );
		break;

	case C_AST_EXPRESSION_LOR:
	case C_AST_EXPRESSION_LAND:
	case C_AST_EXPRESSION_BOR:
	case C_AST_EXPRESSION_BAND:
		ExecuteExpression( node->children[1], c );
		ExecuteExpression( node->children[0], c );
		xi=c.stack.PopInt();
		yi=c.stack.PopInt();
		switch( node->nodeType )
		{
		case C_AST_EXPRESSION_LOR:  ri = xi || yi; break;
		case C_AST_EXPRESSION_LAND: ri = xi && yi; break;
		case C_AST_EXPRESSION_BOR:  ri = xi | yi; break;
		case C_AST_EXPRESSION_BAND: ri = xi & yi; break;
		}
		c.stack.PushInt( ri );
		node->type = C_TYPE_INT;
		break;

	case C_AST_EXPRESSION_LINVERSE:
		ExecuteExpression( node->children[0], c );
		xi = c.stack.PopInt();
		ri = !xi;
		c.stack.PushInt( ri );
		node->type = C_TYPE_INT;
		break;

	case C_AST_EXPRESSION_PLUS:
	case C_AST_EXPRESSION_MINUS:
	case C_AST_EXPRESSION_TIMES:
	case C_AST_EXPRESSION_DIVIDE:
	case C_AST_EXPRESSION_MODULUS:
		ExecuteExpression( node->children[1], c );
		ExecuteExpression( node->children[0], c );
		type = Promote( node->children[0]->type, node->children[1]->type, c );
		if( type == C_TYPE_INT )
		{
			xi = c.stack.PopInt();
			yi = c.stack.PopInt();
			switch( node->nodeType )
			{
			case C_AST_EXPRESSION_PLUS:    ri = xi + yi; break;
			case C_AST_EXPRESSION_MINUS:   ri = xi - yi; break;
			case C_AST_EXPRESSION_TIMES:   ri = xi * yi; break;
			case C_AST_EXPRESSION_DIVIDE:  ri = xi / yi; break; 
			case C_AST_EXPRESSION_MODULUS: ri = xi % yi; break;
			}
			c.stack.PushInt( ri );
		}
		if( type == C_TYPE_FLOAT )
		{
			xf = c.stack.PopFloat();
			yf = c.stack.PopFloat();
			switch( node->nodeType )
			{
			case C_AST_EXPRESSION_PLUS:   rf = xf + yf; break;
			case C_AST_EXPRESSION_MINUS:  rf = xf - yf; break;
			case C_AST_EXPRESSION_TIMES:  rf = xf * yf; break;
			case C_AST_EXPRESSION_DIVIDE: rf = xf / yf; break; 
			}
			c.stack.PushFloat( rf );
		}
		node->type = type;
		break;
	
	case C_AST_EXPRESSION_NEGATIVE:
		ExecuteExpression( node->children[0], c );
		if( BaseType( node->children[0]->type ) == C_TYPE_INT )
		{
			xi = c.stack.PopInt();
			c.stack.PushInt( -xi );
		}
		if( BaseType( node->children[0]->type ) == C_TYPE_FLOAT )
		{
			xf = c.stack.PopFloat();
			c.stack.PushFloat( -xf );
		}
		node->type = BaseType( node->children[0]->type );
		break;

	case C_AST_EXPRESSION_INDEX: 
		ExecuteExpression( node->children[0], c );
		Cast( node->children[0]->type, C_TYPE_INT, c );
		index = c.stack.PopInt();

		for( i = 0 ; i < symbols.size() ; i++ )
			if( symbols[i].name == (char*)node->lexData )
			{
				node->type = symbols[i+index].type;
				c.stack.Push( symbols[i+index].data, TypeSize( symbols[i+index].type ) );
				break;
			}
		break;

	case C_AST_EXPRESSION_BOOLEAN:
		ExecuteExpression( node->children[2], c );
		ExecuteExpression( node->children[0], c );
		type = Promote( node->children[0]->type, node->children[2]->type, c );
		if( type == C_TYPE_INT )
		{
			xi = c.stack.PopInt();
			yi = c.stack.PopInt();
			switch( node->children[1]->nodeType )
			{
			case C_AST_EXPRESSION_BOOLEAN_EQUAL:         ri = ( xi == yi ); break;
			case C_AST_EXPRESSION_BOOLEAN_UNEQUAL:       ri = ( xi != yi ); break;
			case C_AST_EXPRESSION_BOOLEAN_GREATER:       ri = ( xi > yi ); break;
			case C_AST_EXPRESSION_BOOLEAN_LESS:          ri = ( xi < yi ); break;
			case C_AST_EXPRESSION_BOOLEAN_GREATER_EQUAL: ri = ( xi >= yi ); break;
			case C_AST_EXPRESSION_BOOLEAN_LESS_EQUAL:    ri = ( xi <= yi ); break;
			}
		}
		if( type == C_TYPE_FLOAT )
		{
			xf = c.stack.PopFloat();
			yf = c.stack.PopFloat();
			switch( node->children[1]->nodeType )
			{
			case C_AST_EXPRESSION_BOOLEAN_EQUAL:         ri = ( xf == yf ); break;
			case C_AST_EXPRESSION_BOOLEAN_UNEQUAL:       ri = ( xf != yf ); break;
			case C_AST_EXPRESSION_BOOLEAN_GREATER:       ri = ( xf > yf ); break;
			case C_AST_EXPRESSION_BOOLEAN_LESS:          ri = ( xf < yf ); break;
			case C_AST_EXPRESSION_BOOLEAN_GREATER_EQUAL: ri = ( xf >= yf ); break;
			case C_AST_EXPRESSION_BOOLEAN_LESS_EQUAL:    ri = ( xf <= yf ); break;
			}
		}
		c.stack.PushInt( ri );
		node->type = C_TYPE_INT;
		break;

	case C_AST_ID:
		for( i = 0 ; i < symbols.size() ; i++ )
			if( !strcmpi( symbols[i].name.c_str(), (char*)node->lexData ) )
			{
				node->type = symbols[i].type;
				c.stack.Push( symbols[i].data, TypeSize( symbols[i].type ) );
				break;
			}
		break;

	case C_AST_INT:
	case C_AST_HEX:
		xi = *(int*)node->lexData;
		node->type = C_TYPE_INT;
		c.stack.PushInt( xi );
		break;

	case C_AST_FLOAT:
		xf = *(float*)node->lexData;
		node->type = C_TYPE_FLOAT;
		c.stack.PushFloat( xf );
		break;

	case C_AST_VECTOR:
		xv = *(Math::Vector*)node->lexData;
		node->type = C_TYPE_VECTOR;
		c.stack.PushVector( xv );
		break;
	
	case C_AST_STATEMENT_VERBCALL:
		ExecuteVerbCall( node, c, true );
		break;
	}
}
	
void C_Script::Cast( C_SymbolType source, C_SymbolType dest, C_Context &c )
{
	C_SymbolType sourceBase, destBase;
	int intType;
	float floatType;

	if( source==dest ) return;
	sourceBase = BaseType( source );
	destBase = BaseType( dest );

	if( sourceBase==C_TYPE_INT && destBase==C_TYPE_FLOAT )
	{
		intType = c.stack.PopInt();
		c.stack.PushFloat( intType );
	}

	if( sourceBase==C_TYPE_FLOAT && destBase==C_TYPE_INT )
	{
		floatType = c.stack.PopFloat();
		c.stack.PushInt( floatType );
	}
}

C_SymbolType C_Script::Promote( C_SymbolType xType, C_SymbolType yType, C_Context &c )
{
	C_SymbolType resultType;
	C_SymbolType xBaseType, yBaseType;

	int intVar;
	float floatVar;

	xBaseType = BaseType( xType );
	yBaseType = BaseType( yType );

	if( xBaseType==C_TYPE_INT && yBaseType==C_TYPE_INT ) return C_TYPE_INT;
	if( xBaseType==C_TYPE_FLOAT && yBaseType==C_TYPE_FLOAT ) return C_TYPE_FLOAT;

	if( xBaseType==C_TYPE_INT && yBaseType==C_TYPE_FLOAT ) 
	{
		intVar = c.stack.PopInt();
		c.stack.PushFloat( intVar );
		return C_TYPE_FLOAT;
	}

	if( xBaseType==C_TYPE_FLOAT && yBaseType==C_TYPE_INT ) 
	{
		floatVar = c.stack.PopFloat();
		intVar = c.stack.PopInt();
		c.stack.PushFloat( intVar );
		c.stack.PushFloat( floatVar );
		return C_TYPE_FLOAT;
	}
}

