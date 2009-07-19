#include <vector>

#include "C_StateTable.h"
#include "C_Tokenizer.h"
#include "C_AST.h"

#include "G_Console.h"

#include <windows.h>

namespace Cog
{
    ASTNode *ParseString( Tokenizer &t, const string& filename )
    {
	    int state = 0;

	    std::vector<int> stateStack;
	    std::vector<Token> tokenStack;
	    int i = 0;
	    int j;
	    bool stringHack, stringHack2;
	    Token nextToken;
	    Token newToken;
        StateTable::Item sti;

	    char buffer[100];

	    nextToken = t.NextToken( false, false );

	    do
	    {
		    sti = StateTable::getItem( state, nextToken.type );
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
			    BuildASTNode( newToken, &tokenStack[i - sti.ruleSize], sti.number, error );
			    for( j = 0 ; j < sti.ruleSize - 1 ; j++ )
			    {
				    tokenStack.pop_back();
				    stateStack.pop_back();
			    }
			    i -= sti.ruleSize;
    			
			    tokenStack[i] = newToken;
			    state =stateStack[i];
			    sti = StateTable::getItem( state, newToken.type );
			    state = sti.number;
			    i++;
			    break;

		    case UNDEF:
			    //cout << "Parse Error, line " << nextToken.line << endl;
			    wsprintf( buffer, "Error in COG file %s, line %i", filename, nextToken.line );
                Console::Out( buffer );
			    return NULL;
		    }
	    } while( state != StateTable::Accept() );
    return tokenStack[0].astnode;
    }
}

