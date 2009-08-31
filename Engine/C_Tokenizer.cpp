#include <string>

#include "M_Vector.h"
#include "U_Lowercase.h"
#include "C_Tokenizer.h"

extern "C" {
    int yylex();
    void yy_scan_string ( const char *str );
    char *stringval;
    int intval;
    float floatval;
    float vecx;
    float vecy;
    float vecz;
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
		    newToken.lexData.intVal=intval;
            break;

        case 1:
            newToken.type=vectorIndex;
		    newToken.lexData.vectorVal = new Math::Vector(vecx, vecy, vecz);
            break;

        case 2:
            newToken.type=stringIndex;
			newToken.lexData.stringVal=new char[strlen(stringval) - 1];
			strcpy(newToken.lexData.stringVal, stringval+1);
            newToken.lexData.stringVal[strlen(newToken.lexData.stringVal)] = '\0';
            break;

        case 3:
            newToken.type=floatIndex;
		    newToken.lexData.floatVal=floatval;
            break;

        case 4:
            newToken.type=intIndex;
		    newToken.lexData.intVal=intval;
            break;

        case 5:
        case 6:
            for(int i=0;i<numTerminals;i++)
		    {
                if(terminals[i].name == stringval)
			    {
				    newToken.type=terminals[i].index;
                    break;
			    }
		    }
            break;

        case 7:
		    newToken.type=idIndex;
		    newToken.lexData.stringVal=new char[strlen(stringval)+1];
		    strcpy(newToken.lexData.stringVal, stringval);
            break;

        case 8:
            newToken.type=endIndex;
            break;
        }

	    return newToken;
    }
}    	
