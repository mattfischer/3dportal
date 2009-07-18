#ifndef C_TOKENIZER_H
#define C_TOKENIZER_H

#include <string>

using std::string;

namespace Cog
{
    struct ASTNode;

    struct Token
    {
	    int type;
	    void *lexData;
	    ASTNode *astnode;
	    int line;
    };

    struct Terminal
    {
	    string name;
	    int index;
    };

    class Tokenizer
    {
    public:
	    Tokenizer( const string& source);
	    Token NextToken( bool stringHack, bool stringHack2 );
	    bool IsEof() { return eof; }

    protected:
	    int ParseHex(const char *string);

	    int p;
	    int idIndex;
	    int endIndex;
	    int floatIndex;
	    int intIndex;
	    int messageIndex;
	    int hexIndex;
	    int vectorIndex;
	    int stringIndex;

	    std::string data;
	    bool eof;
	    int lineNum;

	    static Terminal terminals[];
	    static int numTerminals;
    };
}

#endif