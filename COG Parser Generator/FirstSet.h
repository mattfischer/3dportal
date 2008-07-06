#ifndef FIRSTSET_H
#define FIRSTSET_H

#include <iostream>
#include <vector>

#include "Token.h"

class FirstSet {
public:
	FirstSet() {};
	FirstSet(const FirstSet &a) { tokens=a.tokens; }
	bool add(Token&);
	bool add(FirstSet&);
	
	int numTokens() { return tokens.size(); }
	Token operator[](int);
	friend ostream &operator<<(ostream&, FirstSet&);
protected:
	vector<Token> tokens;
};

#endif

