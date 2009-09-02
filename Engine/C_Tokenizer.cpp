#include <string>

#include "M_Vector.h"
#include "U_Lowercase.h"
#include "C_Tokenizer.h"
#include "C_ASTNode.h"

extern "C" {
    int yylex();
    void yy_scan_string ( const char *str );
    C_AST_LexData yylval;
    int linenum;
}

namespace Cog
{
    #include "C_Tokenizer.inc"

    Tokenizer::Tokenizer( const string& source )
    {
	    lineNum = 1;
        data = Util::Lowercase(source);

	    for( int i = 0 ; i < numTerminals ; i++ )
	    {
		    if( terminals[i].name == "<id>" )      idIndex = terminals[i].index;
		    if( terminals[i].name == "<end>" )     endIndex = terminals[i].index;
		    if( terminals[i].name == "<float>" )   floatIndex = terminals[i].index;
		    if( terminals[i].name == "<int>" )     intIndex = terminals[i].index;
		    if( terminals[i].name == "<hex>" )     hexIndex = terminals[i].index;
		    if( terminals[i].name == "<vector>" )  vectorIndex = terminals[i].index;
		    if( terminals[i].name == "<string>" )  stringIndex = terminals[i].index;
	    }

        yy_scan_string( data.c_str() );
    }

    Token Tokenizer::NextToken(bool stringHack, bool stringHack2)
    {
	    Token newToken;
     
        linenum = lineNum;
        int token = yylex();

	    newToken.line=lineNum;

        switch(token)
        {
        case 0:
            newToken.type=hexIndex;
		    break;

        case 1:
            newToken.type=vectorIndex;
            break;

        case 2:
            newToken.type=stringIndex;
            break;

        case 3:
            newToken.type=floatIndex;
            break;

        case 4:
            newToken.type=intIndex;
            break;

        case 5:
        case 6:
            for(int i=0;i<numTerminals;i++)
		    {
                if(terminals[i].name == yylval.stringVal)
			    {
				    newToken.type=terminals[i].index;
                    break;
			    }
		    }
            break;

        case 7:
		    newToken.type=idIndex;
            break;

        case 8:
            newToken.type=endIndex;
            break;
        }

        newToken.lexData = yylval;
	    return newToken;
    }
}    	
