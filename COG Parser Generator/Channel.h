#ifndef CHANNEL_H
#define CHANNEL_H

using namespace std;

struct ChannelNode 
{
	int set;
	int rule;
};

class Channel
{
public:
	Channel() {};
	Channel(const Channel &copy) : in(copy.in), out(copy.out), type(copy.type) {}
	Channel(int inset, int inrule, int outset, int outrule, int ctype) 
	{
	in.set=inset;
	in.rule=inrule;
	out.set=outset;
	out.rule=outrule;
	type=ctype;
	}


	ChannelNode in;
	ChannelNode out;
	int type;
};

#endif