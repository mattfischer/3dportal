#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <vector>
#include <string>

struct ASTNode;
enum RelopType;

using namespace std;

class Token 
{
public:
	Token();
	Token(int a, string n, bool t);
	Token(const Token &c);
	
	string getName() { return name; }
	bool isTerminal() { return terminal; }
	int getType() { return type; }
	int num() { return (int)type; }
	bool operator==(const Token &b) const;
	bool operator!=(const Token &b) const;
	friend ostream& operator<<(ostream &c, Token &t);

	double numVal;
	string strVal;
	RelopType relType;
	int line;

	ASTNode *astnode;
protected:
	bool terminal;
	int type;
	string name;
};

#endif