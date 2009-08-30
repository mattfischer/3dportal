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
        newToken.lexData = NULL;

        switch(token)
        {
        case 0:
            newToken.type=hexIndex;
		    newToken.lexData=new int;
		    *(int*)newToken.lexData=intval;
            break;

        case 1:
            newToken.type=vectorIndex;
		    newToken.lexData=new Math::Vector(vecx, vecy, vecz);
            break;

        case 2:
            newToken.type=stringIndex;
			newToken.lexData=new char[strlen(stringval) - 1];
			strcpy((char*)newToken.lexData, stringval+1);
            ((char*)newToken.lexData)[strlen((char*)newToken.lexData)] = '\0';
            break;

        case 3:
            newToken.type=floatIndex;
		    newToken.lexData=(void*)new float;
		    *((float*)newToken.lexData)=floatval;
            break;

        case 4:
            newToken.type=intIndex;
		    newToken.lexData=(void*)new int;
		    *((int*)newToken.lexData)=intval;
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
		    newToken.lexData=new char[strlen(stringval)+1];
		    strcpy((char*)newToken.lexData, stringval);
            break;

        case 8:
            newToken.type=endIndex;
            break;
        }

	    return newToken;
    }
}    	
