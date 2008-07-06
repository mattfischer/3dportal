#ifndef C_STATETABLE_H
#define C_STATETABLE_H

enum C_SetAction { UNDEF, SHIFT, REDUCE };

struct C_StateTableItem {
	C_SetAction action;
	int number;
	int ruleSize;
	int newToken;
};

class C_StateTable
{
public:
	C_StateTable() {}
	static C_StateTableItem Item( int state, int token ) { return table[state*numTokens+token]; }
	static int Accept() { return accept; }
protected:
	static C_StateTableItem table[];
	static int numTokens;
	static int numStates;
	static int accept;
};


#endif