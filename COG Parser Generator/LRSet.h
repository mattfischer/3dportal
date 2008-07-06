#ifndef LRSET_H
#define LRSET_H

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "Rule.h"
#include "FirstSet.h"
#include "Channel.h"

using namespace std;


class ShiftLink 
{
public:
	ShiftLink() {};
	ShiftLink(const ShiftLink&);
	Token token;
	int shiftState;
};

class LRSet 
{
public:
	LRSet() { };
	LRSet(const LRSet&);

	int findRule(PCRule&);
	void addPCRule(PCRule&);
	void addLink(ShiftLink&);
	void populateSet(vector<Rule>&, vector<FirstSet>&, vector<Channel>&);
	void clear();
	bool isLink(Token&);
	int numRules();
	
	PCRule &operator[](int);
	ShiftLink &operator[](Token&);
	bool operator==(LRSet&);
	friend ostream &operator<<(ostream&, LRSet&);
	
protected:
	vector<PCRule> rules;
	vector<ShiftLink> links;
};
#endif