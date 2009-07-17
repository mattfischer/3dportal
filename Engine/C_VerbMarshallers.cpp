#include "C_Script.h"
#include "C_Stack.h"
#include "C_Verb.h"
#include "JK_Level.h"
#include "W_Thing.h"
#include "G_Console.h"

extern R_Model* povModel;

void C_Script::ExecuteVerbCall( C_ASTNode *node, C_Context &c, bool expression )
{
	int i;
	char buffer[100];

	if( !strcmp( (char*)node->lexData, "sendmessage" ) ) return; // ****
			
	for( i = node->numChildren - 1 ; i >= 0 ; i-- )
		ExecuteExpression( node->children[i], c );

#include "C_VerbMarshallers.inc"

	for( i = 0 ; i < node->numChildren ; i++ )
	{
		switch( BaseType( node->children[i]->type ) )
		{
		case C_TYPE_INT:
			c.stack.PopInt();
			break;
		case C_TYPE_FLOAT:
			c.stack.PopFloat();
			break;
		case C_TYPE_VECTOR:
			c.stack.PopVector();
			break;
		}
	}

	wsprintf( buffer, "Call to undefined verb %s", node->lexData );
	CON_Out( buffer );
}