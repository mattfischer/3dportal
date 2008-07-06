#ifndef C_TOKENIZER_H
#define C_TOKENIZER_H

#include <string>

struct C_ASTNode;

using std::string;

struct C_Token
{
	int type;
	void *lexData;
	C_ASTNode *astnode;
	int line;
};

struct C_Terminal
{
	string name;
	int index;
};

class C_Tokenizer
{
public:
	C_Tokenizer( const string& source);
	C_Token NextToken( bool stringHack, bool stringHack2 );
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

	static C_Terminal terminals[];
	static int numTerminals;
};

#endif