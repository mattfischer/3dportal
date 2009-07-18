#ifndef C_STATETABLE_H
#define C_STATETABLE_H

namespace Cog
{
    enum SetAction { UNDEF, SHIFT, REDUCE };

    class StateTable
    {
    public:
        struct Item {
	        SetAction action;
	        int number;
	        int ruleSize;
	        int newToken;
        };

	    StateTable() {}
	    static Item getItem( int state, int token ) { return table[state*numTokens+token]; }
	    static int Accept() { return accept; }
    protected:
	    static Item table[];
	    static int numTokens;
	    static int numStates;
	    static int accept;
    };
}

#endif