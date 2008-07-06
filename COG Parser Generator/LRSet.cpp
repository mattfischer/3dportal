#include "LRSet.h"

LRSet::LRSet(const LRSet &a)
{
	rules=a.rules;
	links=a.links;
}

int LRSet::findRule(PCRule &a)
{
	int i;

	for(i=0;i<rules.size();i++) if(a==rules[i]) return i;
	return -1;
}

bool LRSet::operator==(LRSet &a)
{
	int i;

	if(rules.size()!=a.rules.size()) return false;
	for(i=0;i<rules.size();i++) if(findRule(a.rules[i])==-1) return false;
	return true;
}

void LRSet::populateSet(vector<Rule> &ruleSet, vector<FirstSet> &firstSets, vector<Channel> &channels)
{
	int i,j,k;
	Token temp;
	int r;
	PCRule newrule;
	vector<Token> lookahead;
	Token laToken;
	Channel tempchannel;

	for(i=0;i<rules.size();i++)
	{
		if(rules[i].complete()==false)
		{
			temp=rules[i].nextToken();	
			lookahead.clear();

			if(rules[i].nextTokenIndex()==rules[i].length()-1) 
				lookahead=rules[i].lookAhead();
			else
			{
				laToken=rules[i].twoNext();
				
				if(laToken.isTerminal()) lookahead.push_back(laToken);
				else
				{
					for(j=0;j<firstSets[laToken.num()].numTokens();j++)
						lookahead.push_back(firstSets[laToken.num()][j]);
				}
			}
		
			tempchannel.in.set=-1;
			tempchannel.in.rule=i;
			tempchannel.out.set=-1;
			tempchannel.out.rule=-1;
			tempchannel.type=1;
			for(j=0;j<ruleSet.size();j++)
				if(ruleSet[j].leftToken()==temp)
				{
					newrule=PCRule(ruleSet[j],j,lookahead);	
					r=findRule(newrule);
					if(r==-1) 
					{
						tempchannel.out.rule=rules.size();
						addPCRule(newrule);
					}
					else
					{
						rules[r].addLookahead(newrule);
						tempchannel.out.rule=r;
					}
				channels.push_back(tempchannel);
			}
		}
	}
}
	
void LRSet::addPCRule(PCRule &a)
{
	rules.push_back(a);
}

ostream &operator<<(ostream &c, LRSet &l)
{
	int i;

	for(i=0;i<l.rules.size();i++) 
		c << l.rules[i] << endl;
	c << endl;

#ifdef FULLPRINT
	for(i=0;i<l.links.size();i++) 
		c << l.links[i].token << "->" << l.links[i].shiftState << endl;
#endif
	return c;
}

void LRSet::clear()
{
	rules.clear();
	links.clear();
}

void LRSet::addLink(ShiftLink &a)
{
	links.push_back(a);
}

bool LRSet::isLink(Token &a)
{
	int i;

	for(i=0;i<links.size();i++) if(links[i].token==a) return true;
	return false;
}

PCRule &LRSet::operator[](int i)
{
	return rules[i];
}

ShiftLink &LRSet::operator[](Token &a)
{
	int i;

	for(i=0;i<links.size();i++) if(links[i].token==a) return links[i];
}

int LRSet::numRules()
{
	return rules.size();
}

ShiftLink::ShiftLink(const ShiftLink &a) : token(a.token)
{
	shiftState=a.shiftState;
}