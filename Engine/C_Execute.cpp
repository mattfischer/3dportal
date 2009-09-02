#include "C_Script.h"
#include "C_Stack.h"
#include "C_Type.h"
#include "M_Vector.h"

#include "G_Console.h"

namespace Cog
{
    void Script::ExecuteStatements( int start, Context &c )
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

    void Script::ExecuteKickstart( void *object )
    {
	    Context c;
	    int statement;
	    int i;
	    bool found;

	    c.sender = ( (Script*)object )->startInfo.sender;
	    c.senderType=( (Script*)object )->startInfo.senderType;
	    c.source=( (Script*)object )->startInfo.source;
	    c.touched=( (Script*)object )->startInfo.touched;

	    found = false;

	    for( i = 0 ; i < ( (Script*)object )->symbols.size() ; i++ )
		    if( ( (Script*)object )->symbols[i].type == c.senderType && ( (Script*)object )->symbols[i].data.intVal == c.sender )
		    {
			    c.senderId = ( (Script*)object )->symbols[i].linkId;
			    found = true;
			    break;
		    }
	    c.senderId = -1;

	    srand( GetCurrentThreadId() * rand() );
	    statement = ( (Script*)object )->startInfo.statement;
	    ReleaseSemaphore( semaphore, 1, NULL );
	    ( (Script*)object )->ExecuteStatements( statement, c );
    }

    bool Script::ExecuteStatement( ASTNode *node, Context &c )
    {
	    int i;
	    int start;
	    bool statementReturn;
	    char buffer[100];
	    int x;
	    int status;

	    switch( node->nodeType )
	    {
	    case NODE_STATEMENT_LIST: 
		    for( i = 0 ; i < node->numChildren ; i++ )
		    {
			    statementReturn = ExecuteStatement( node->children[i], c );
			    if( statementReturn ) return true;
		    }
		    return false;

	    case NODE_STATEMENT_CALL: 
		    for( i = 0 ; i < messages.size() ; i++ )
			    if( messages[i].name == node->lexData.stringVal )
				    ExecuteStatements( messages[i].statementStart, c );
		    return false;

	    case NODE_STATEMENT_ASSIGN: 
	    case NODE_STATEMENT_ASSIGN_INDEXED: 
		    ExecuteAssign( node, c );
		    return false;

	    case NODE_STATEMENT_IF: 
	    case NODE_STATEMENT_IF_ELSE:
		    ExecuteExpression( node->children[0], c );
		    x = c.stack.PopInt();
		    statementReturn = false;
		    if( x ) statementReturn = ExecuteStatement( node->children[1], c );
		    else if( node->nodeType == NODE_STATEMENT_IF_ELSE ) 
			    statementReturn = ExecuteStatement( node->children[2], c );
		    return statementReturn;

	    case NODE_STATEMENT_FOR: 
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

	    case NODE_STATEMENT_WHILE: 
		    while( 1 )
		    {
			    ExecuteExpression( node->children[0], c );
			    status = c.stack.PopInt();
			    if( status == 0 ) break;
			    statementReturn = ExecuteStatement( node->children[1], c );
			    if( statementReturn ) return true;
		    }
		    return false;

	    case NODE_STATEMENT_VERBCALL: 
		    ExecuteVerbCall( node, c, false );
		    return false;

	    case NODE_STATEMENT_RETURN: 
		    return true;
    	
	    case NODE_STATEMENT_LABEL: 
	    case NODE_STATEMENT_EMPTY: 
		    return false;
	    }

	    return false;
    }

    void Script::ExecuteAssign( ASTNode *node, Context &c )
    {
	    int i;
	    char buffer[100];
	    int index;

	    switch( node->nodeType )
	    {
	    case NODE_STATEMENT_ASSIGN:
	    case NODE_EXPRESSION_ASSIGN:
		    ExecuteExpression( node->children[0], c );
		    for( i = 0 ; i < symbols.size() ; i++ )
			    if( !strcmpi( symbols[i].name.c_str(), node->lexData.stringVal ) )
			    {
				    Cast( node->children[0]->type, symbols[i].type, c );
				    c.stack.Pop( &symbols[i].data, TypeSize( symbols[i].type ) );
				    break;
			    }
		    break;
	    case NODE_STATEMENT_ASSIGN_INDEXED:
	    case NODE_EXPRESSION_ASSIGN_INDEXED:
		    ExecuteExpression( node->children[1], c );
		    ExecuteExpression( node->children[0], c );
		    index = c.stack.PopInt();
		    for( i = 0 ; i < symbols.size() ; i++ )
			    if( symbols[i].name == node->lexData.stringVal )
			    {
				    Cast( node->children[0]->type, symbols[i+index].type, c );
				    c.stack.Pop( &symbols[i].data, TypeSize( symbols[i+index].type ) );
				    break;
			    }
		    break;
	    }
    	
    }

    int Script::TypeSize(SymbolType type)
    {
	    switch(type)
	    {
		    case TYPE_FLEX: 
		    case TYPE_FLOAT:
			    return sizeof(float);
    		
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
			    return sizeof(int);

		    case TYPE_VECTOR: 
			    return sizeof(Math::Vector);
	    }
    }

    SymbolType Script::BaseType(SymbolType type)
    {
	    switch(type)
	    {
		    case TYPE_FLEX: 
		    case TYPE_FLOAT:
			    return TYPE_FLOAT;
    		
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
			    return TYPE_INT;

		    case TYPE_VECTOR: 
			    return TYPE_VECTOR;
	    }
    }


    void Script::ExecuteExpression( ASTNode *node, Context &c )
    {
	    int i;
	    int xi,yi, ri;
	    float xf, yf, rf;
	    Math::Vector xv;
	    SymbolType type;
	    int index;

	    switch( node->nodeType )
	    {
	    case NODE_EXPRESSION_ASSIGN:
	    case NODE_EXPRESSION_ASSIGN_INDEXED: 
		    ExecuteAssign( node, c );
		    break;

	    case NODE_EXPRESSION_LOR:
	    case NODE_EXPRESSION_LAND:
	    case NODE_EXPRESSION_BOR:
	    case NODE_EXPRESSION_BAND:
		    ExecuteExpression( node->children[1], c );
		    ExecuteExpression( node->children[0], c );
		    xi=c.stack.PopInt();
		    yi=c.stack.PopInt();
		    switch( node->nodeType )
		    {
		    case NODE_EXPRESSION_LOR:  ri = xi || yi; break;
		    case NODE_EXPRESSION_LAND: ri = xi && yi; break;
		    case NODE_EXPRESSION_BOR:  ri = xi | yi; break;
		    case NODE_EXPRESSION_BAND: ri = xi & yi; break;
		    }
		    c.stack.PushInt( ri );
		    node->type = TYPE_INT;
		    break;

	    case NODE_EXPRESSION_LINVERSE:
		    ExecuteExpression( node->children[0], c );
		    xi = c.stack.PopInt();
		    ri = !xi;
		    c.stack.PushInt( ri );
		    node->type = TYPE_INT;
		    break;

	    case NODE_EXPRESSION_PLUS:
	    case NODE_EXPRESSION_MINUS:
	    case NODE_EXPRESSION_TIMES:
	    case NODE_EXPRESSION_DIVIDE:
	    case NODE_EXPRESSION_MODULUS:
		    ExecuteExpression( node->children[1], c );
		    ExecuteExpression( node->children[0], c );
		    type = Promote( node->children[0]->type, node->children[1]->type, c );
		    if( type == TYPE_INT )
		    {
			    xi = c.stack.PopInt();
			    yi = c.stack.PopInt();
			    switch( node->nodeType )
			    {
			    case NODE_EXPRESSION_PLUS:    ri = xi + yi; break;
			    case NODE_EXPRESSION_MINUS:   ri = xi - yi; break;
			    case NODE_EXPRESSION_TIMES:   ri = xi * yi; break;
			    case NODE_EXPRESSION_DIVIDE:  ri = xi / yi; break; 
			    case NODE_EXPRESSION_MODULUS: ri = xi % yi; break;
			    }
			    c.stack.PushInt( ri );
		    }
		    if( type == TYPE_FLOAT )
		    {
			    xf = c.stack.PopFloat();
			    yf = c.stack.PopFloat();
			    switch( node->nodeType )
			    {
			    case NODE_EXPRESSION_PLUS:   rf = xf + yf; break;
			    case NODE_EXPRESSION_MINUS:  rf = xf - yf; break;
			    case NODE_EXPRESSION_TIMES:  rf = xf * yf; break;
			    case NODE_EXPRESSION_DIVIDE: rf = xf / yf; break; 
			    }
			    c.stack.PushFloat( rf );
		    }
		    node->type = type;
		    break;
    	
	    case NODE_EXPRESSION_NEGATIVE:
		    ExecuteExpression( node->children[0], c );
		    if( BaseType( node->children[0]->type ) == TYPE_INT )
		    {
			    xi = c.stack.PopInt();
			    c.stack.PushInt( -xi );
		    }
		    if( BaseType( node->children[0]->type ) == TYPE_FLOAT )
		    {
			    xf = c.stack.PopFloat();
			    c.stack.PushFloat( -xf );
		    }
		    node->type = BaseType( node->children[0]->type );
		    break;

	    case NODE_EXPRESSION_INDEX: 
		    ExecuteExpression( node->children[0], c );
		    Cast( node->children[0]->type, TYPE_INT, c );
		    index = c.stack.PopInt();

		    for( i = 0 ; i < symbols.size() ; i++ )
			    if( symbols[i].name == node->lexData.stringVal )
			    {
				    node->type = symbols[i+index].type;
				    c.stack.Push( &symbols[i+index].data, TypeSize( symbols[i+index].type ) );
				    break;
			    }
		    break;

	    case NODE_EXPRESSION_BOOLEAN:
		    ExecuteExpression( node->children[2], c );
		    ExecuteExpression( node->children[0], c );
		    type = Promote( node->children[0]->type, node->children[2]->type, c );
		    if( type == TYPE_INT )
		    {
			    xi = c.stack.PopInt();
			    yi = c.stack.PopInt();
			    switch( node->children[1]->nodeType )
			    {
			    case NODE_EXPRESSION_BOOLEAN_EQUAL:         ri = ( xi == yi ); break;
			    case NODE_EXPRESSION_BOOLEAN_UNEQUAL:       ri = ( xi != yi ); break;
			    case NODE_EXPRESSION_BOOLEAN_GREATER:       ri = ( xi > yi ); break;
			    case NODE_EXPRESSION_BOOLEAN_LESS:          ri = ( xi < yi ); break;
			    case NODE_EXPRESSION_BOOLEAN_GREATER_EQUAL: ri = ( xi >= yi ); break;
			    case NODE_EXPRESSION_BOOLEAN_LESS_EQUAL:    ri = ( xi <= yi ); break;
			    }
		    }
		    if( type == TYPE_FLOAT )
		    {
			    xf = c.stack.PopFloat();
			    yf = c.stack.PopFloat();
			    switch( node->children[1]->nodeType )
			    {
			    case NODE_EXPRESSION_BOOLEAN_EQUAL:         ri = ( xf == yf ); break;
			    case NODE_EXPRESSION_BOOLEAN_UNEQUAL:       ri = ( xf != yf ); break;
			    case NODE_EXPRESSION_BOOLEAN_GREATER:       ri = ( xf > yf ); break;
			    case NODE_EXPRESSION_BOOLEAN_LESS:          ri = ( xf < yf ); break;
			    case NODE_EXPRESSION_BOOLEAN_GREATER_EQUAL: ri = ( xf >= yf ); break;
			    case NODE_EXPRESSION_BOOLEAN_LESS_EQUAL:    ri = ( xf <= yf ); break;
			    }
		    }
		    c.stack.PushInt( ri );
		    node->type = TYPE_INT;
		    break;

	    case NODE_ID:
		    for( i = 0 ; i < symbols.size() ; i++ )
			    if( !strcmpi( symbols[i].name.c_str(), node->lexData.stringVal ) )
			    {
				    node->type = symbols[i].type;
				    c.stack.Push( &symbols[i].data, TypeSize( symbols[i].type ) );
				    break;
			    }
		    break;

	    case NODE_INT:
	    case NODE_HEX:
		    xi = node->lexData.intVal;
		    node->type = TYPE_INT;
		    c.stack.PushInt( xi );
		    break;

	    case NODE_FLOAT:
		    xf = node->lexData.floatVal;
		    node->type = TYPE_FLOAT;
		    c.stack.PushFloat( xf );
		    break;

	    case NODE_VECTOR:
            xv = Math::Vector(node->lexData.vectorVal.x, node->lexData.vectorVal.y, node->lexData.vectorVal.z);
		    node->type = TYPE_VECTOR;
		    c.stack.PushVector( xv );
		    break;
    	
	    case NODE_STATEMENT_VERBCALL:
		    ExecuteVerbCall( node, c, true );
		    break;
	    }
    }
    	
    void Script::Cast( SymbolType source, SymbolType dest, Context &c )
    {
	    SymbolType sourceBase, destBase;
	    int intType;
	    float floatType;

	    if( source==dest ) return;
	    sourceBase = BaseType( source );
	    destBase = BaseType( dest );

	    if( sourceBase==TYPE_INT && destBase==TYPE_FLOAT )
	    {
		    intType = c.stack.PopInt();
		    c.stack.PushFloat( intType );
	    }

	    if( sourceBase==TYPE_FLOAT && destBase==TYPE_INT )
	    {
		    floatType = c.stack.PopFloat();
		    c.stack.PushInt( floatType );
	    }
    }

    SymbolType Script::Promote( SymbolType xType, SymbolType yType, Context &c )
    {
	    SymbolType resultType;
	    SymbolType xBaseType, yBaseType;

	    int intVar;
	    float floatVar;

	    xBaseType = BaseType( xType );
	    yBaseType = BaseType( yType );

	    if( xBaseType==TYPE_INT && yBaseType==TYPE_INT ) return TYPE_INT;
	    if( xBaseType==TYPE_FLOAT && yBaseType==TYPE_FLOAT ) return TYPE_FLOAT;

	    if( xBaseType==TYPE_INT && yBaseType==TYPE_FLOAT ) 
	    {
		    intVar = c.stack.PopInt();
		    c.stack.PushFloat( intVar );
		    return TYPE_FLOAT;
	    }

	    if( xBaseType==TYPE_FLOAT && yBaseType==TYPE_INT ) 
	    {
		    floatVar = c.stack.PopFloat();
		    intVar = c.stack.PopInt();
		    c.stack.PushFloat( intVar );
		    c.stack.PushFloat( floatVar );
		    return TYPE_FLOAT;
	    }
    }
}
