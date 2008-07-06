#ifndef STATETABLE_H
#define STATETABLE_H

#include <iostream>
#include <iomanip>
#include <string>

#include "Token.h"
#include "LRSet.h"

using namespace std;

enum SetAction { UNDEF, SHIFT, REDUCE };

struct StateTableItem {
	SetAction action;
	int number;
	int ruleSize;
	int newToken;
};

class StateTable
{
public:
	StateTable(int p_numStates, int p_numTokens, int p_accept);
	StateTable(char *filename);
	~StateTable();
	StateTableItem *operator[](int n);
	void Print(vector<Token> &tokens);
	void Save(char *filename);
	int AcceptState() { return accept; }
protected:
	void GenerateTable(vector<LRSet> &sets);

	int accept;
	int numStates;
	int numTokens;
	StateTableItem *table;
};

#endif