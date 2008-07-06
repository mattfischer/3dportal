#include "Token.h"

Token::Token()
{
	terminal=true;
	name="";
	type=0;
}

Token::Token(int a, string n, bool t)
{
	int i;

	terminal=t;
	name=n;
	type=a;
}

Token::Token(const Token &c)
{
	terminal=c.terminal;
	type=c.type;
	name=c.name;
	astnode=c.astnode;
	strVal=c.strVal;
	numVal=c.numVal;
	relType=c.relType;
}

bool Token::operator==(const Token &b) const
{
	if(type==b.type) return true;
	else			 return false;
}

bool Token::operator!=(const Token &b) const
{
	if(type!=b.type) return true;
	else			 return false;
}


ostream& operator<<(ostream &c, Token &t)
{
	int i;

	c << t.getName();

	return c;
}

