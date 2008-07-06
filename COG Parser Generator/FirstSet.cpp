#include "FirstSet.h"

bool FirstSet::add(Token &a)
{
	int i;
		for(i=0;i<tokens.size();i++) if(a==tokens[i]) return false;
	tokens.push_back(a);
	return true;
}

bool FirstSet::add(FirstSet &a)
{
	int i;
	bool changedSet=false;

	for(i=0;i<a.tokens.size();i++) if(add(a.tokens[i])==true) changedSet=true;
	
	return changedSet;
}
	
Token FirstSet::operator[](int n)
{
	return tokens[n];
}
	
ostream &operator<<(ostream &c, FirstSet &f)
{
	int i;

	for(i=0;i<f.tokens.size();i++) c << f.tokens[i] << " ";

	return c;
}
