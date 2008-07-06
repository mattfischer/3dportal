#define FULLPRINT

#ifndef RULE_H
#define RULE_H

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "FirstSet.h"

using namespace std;

class Rule
{
public:
	Rule(Token &a, Token *b, int n);
	Rule() {};
	Rule(const Rule &r);

	bool match(vector<Token> &test);
	int length();
	Token leftToken();
	Token operator[](int n) { return right[n];}
	
	friend ostream &operator<<(ostream &c, Rule &r);

protected:
	Token left;
	vector<Token> right;
};

class PCRule : public Rule
{
public:
	PCRule() {}; 
	PCRule(Token &a, Token *b, int n, int p, Token t) : Rule(a,b,n), nextIndex(0), parentRule(p) {laTokens.push_back(t);};
	PCRule(const PCRule &r);
	PCRule(Rule &r, int p, vector<Token> &t): Rule(r), nextIndex(0), parentRule(p), laTokens(t) {};
	PCRule(Rule &r, int p, Token &t): Rule(r), nextIndex(0), parentRule(p) { laTokens.push_back(t); };
	PCRule(Rule &r, int p, FirstSet &f): Rule(r), nextIndex(0), parentRule(p)
	{
		for(int i=0;i<f.numTokens();i++) laTokens.push_back(f[i]);
	}

	Token nextToken();
	Token twoNext();
	int nextTokenIndex() { return nextIndex; }
	bool complete();
	bool hasLookahead(Token&);
	bool addLookahead(Token&);
	bool addLookahead(PCRule&);
	bool addLookahead(FirstSet&);
	bool addLookahead(vector<Token>&);
	int ruleNum();
	vector<Token> &lookAhead();

	PCRule &operator=(Rule &r);
	void operator++();
	bool operator==(PCRule&);
	friend ostream &operator<<(ostream&, PCRule&);

protected:
	int nextIndex;
	int parentRule;
	vector<Token> laTokens;
};
#endif