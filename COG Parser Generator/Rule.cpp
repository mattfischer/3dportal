#include "Rule.h"

Rule::Rule(Token &a, Token *b, int n)
{
	int i;

	left=a;

	for(i=0;i<n;i++) right.push_back(b[i]);
}
	
Rule::Rule(const Rule &r)
{
	left=r.left;
	right=r.right;
}


bool Rule::match(vector<Token> &test)
{
	int i,i2;
	if(test.size()<right.size()) return false;

	for(i=0, i2=test.size()-right.size();i<right.size();i++, i2++)
	{
		if(right[i]!=test[i2]) return false;
	}
	return true;
}

int Rule::length()
{
	return right.size();
}

Token Rule::leftToken()
{
	return left;
}

ostream &operator<<(ostream &c, Rule &r)
{
	int i;

	c << r.left << ": ";
	for(i=0;i<r.right.size();i++) c << r.right[i] << " ";

	return c;
}

void PCRule::operator++()
{
	if(nextIndex<right.size()) nextIndex++;
}

Token PCRule::nextToken()
{
	return right[nextIndex];
}

Token PCRule::twoNext()
{
	return right[nextIndex+1];
}

bool PCRule::complete()
{
	return (nextIndex==right.size());
}

bool PCRule::operator==(PCRule &a)
{
	int i;

	if(left!=a.left) return false;
	if(right!=a.right) return false;
	if(nextIndex!=a.nextIndex) return false; 

	return true;
}

bool PCRule::hasLookahead(Token &t)
{
	int i;

	for(i=0;i<laTokens.size();i++) if(laTokens[i]==t) return true;
	return false;
}

bool PCRule::addLookahead(Token &t)
{
	if(!hasLookahead(t))
	{
		laTokens.push_back(t);
		return true;
	}

	return false;
}

bool PCRule::addLookahead(PCRule &r)
{
	int i;
	bool added=false;

	for(i=0;i<r.laTokens.size();i++)
	{
		if(addLookahead(r.laTokens[i])==true) added=true;
	}

	return added;
}

bool PCRule::addLookahead(FirstSet &f)
{
	int i;
	bool added=false;

	for(i=0;i<f.numTokens();i++)
	{
		if(addLookahead(f[i])==true) added=true;
	}
	return added;
}

bool PCRule::addLookahead(vector<Token> &l)
{
	int i;
	bool added=false;

	for(i=0;i<l.size();i++)
	{
		if(addLookahead(l[i])==true) added=true;
	}
	return added;
}

PCRule::PCRule(const PCRule &a)
{
	left=a.left;
	right=a.right;	
	nextIndex=a.nextIndex;
	parentRule=a.parentRule;
	laTokens=a.laTokens;
}

ostream &operator<<(ostream &c, PCRule &r)
{
	int i;

	c << r.left << ": ";
	for(i=0;i<r.right.size();i++)
	{
		if(r.nextIndex==i) c << ". ";
		c << r.right[i] << " ";
	}

	if(r.nextIndex==r.right.size()) c << ".";
#ifdef FULLPRINT
	c << " [";
	for(i=0;i<r.laTokens.size();i++)
	{
		c << r.laTokens[i];
		if(i!=r.laTokens.size()-1) c << " ";
	}
	c << "]";
#endif
	return c;
}

int PCRule::ruleNum()
{
	return parentRule;
}

vector<Token> &PCRule::lookAhead()
{
	return laTokens;
}