#include <vector>

#include "C_StateTable.h"
#include "C_Tokenizer.h"
#include "C_AST.h"

#include "G_Console.h"

#include <windows.h>

C_ASTNode *C_ParseString( C_Tokenizer &t, const string& filename )
{
	int state = 0;

	std::vector<int> stateStack;
	std::vector<C_Token> tokenStack;
	int i = 0;
	int j;
	bool stringHack, stringHack2;
	C_Token nextToken;
	C_Token newToken;
	C_StateTableItem sti;

	char buffer[100];

	nextToken = t.NextToken( false, false );

	do
	{
		sti = C_StateTable::Item( state, nextToken.type );
		switch( sti.action )
		{
		case SHIFT:
			stateStack.push_back( state );
			tokenStack.push_back( nextToken );
			i++;
			if( ( stateStack.size() > 1 && stateStack[1] == 4 ) || 
				( stateStack.size() > 4 && stateStack[4] == 4)) stringHack = true;
			else	stringHack = false;
			//if(state==50) stringHack2=true;
			//else	stringHack2=false;
			stringHack2 = false;
			state = sti.number;
			nextToken = t.NextToken( stringHack, stringHack2 );
			break;

		case REDUCE:
			int error;
			newToken.type = sti.newToken;
			C_BuildASTNode( newToken, &tokenStack[i - sti.ruleSize], sti.number, error );
			for( j = 0 ; j < sti.ruleSize - 1 ; j++ )
			{
				tokenStack.pop_back();
				stateStack.pop_back();
			}
			i -= sti.ruleSize;
			
			tokenStack[i] = newToken;
			state =stateStack[i];
			sti = C_StateTable::Item( state, newToken.type );
			state = sti.number;
			i++;
			break;

		case UNDEF:
			//cout << "Parse Error, line " << nextToken.line << endl;
			wsprintf( buffer, "Error in COG file %s, line %i", filename, nextToken.line );
			CON_Out( buffer );
			return NULL;
		}
	} while( state != C_StateTable::Accept() );
return tokenStack[0].astnode;
}

/*void main()
{
	FILE *file;
	char *data;
	int size;

	file=fopen("actor_ry.cog", "rb");
	fseek(file, 0, SEEK_END);
	size=ftell(file);
	fseek(file,0,SEEK_SET);
	data=new char[size+1];
	fread(data, 1, size, file);
	data[size]='\0';
	fclose(file);

	C_Tokenizer t(data);

	C_ASTNode *node = ParseString(t);
	
	C_DeleteAST(node);
}*/


