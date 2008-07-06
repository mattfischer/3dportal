#include <iostream>
#include <fstream>
#include <string>

#include "Token.h"
#include "Rule.h"
#include "LRSet.h"
#include "FirstSet.h"
#include "StateTable.h"

using namespace std;

vector<Rule> rules;
vector<Token> tokens;

void generateParser(vector<Rule> &rules, vector<Token> &tokens, Token start);

Token FindOrAddToken(string name, bool terminal)
{
	int i;
	
	for(i=0;i<tokens.size();i++)
		if(tokens[i].getName()==name) return tokens[i];
	
	i=tokens.size();
	tokens.push_back(Token(i, name, terminal));
	return tokens[i];
}

bool isTerminal(string str)
{
	if(str[0]>='A' && str[0]<='Z') return false;
	else return true;
}

void parseRule(string ruleSource)
{
	int i,j,k;
	Token ruleLeft;
	vector<Token> ruleRight;
	string substring;

	j=0;
	k=j;
	while(ruleSource[k]!=':') k++;
	substring=ruleSource.substr(j,k-j);
	j=k+1;
	
	ruleLeft=FindOrAddToken(substring, isTerminal(substring));
	
	while(1)
	{
		while(ruleSource[j]==' ') j++;
		if(ruleSource[j]=='\0') break;
		
		k=j+1;
		while(ruleSource[k]!=' ' && ruleSource[k]!='\0') k++;
		substring=ruleSource.substr(j,k-j);
		j=k;
		
		ruleRight.push_back(FindOrAddToken(substring, isTerminal(substring)));
	}
	rules.push_back(Rule(ruleLeft,&ruleRight[0], ruleRight.size()));
}

void generateSets(vector<LRSet> &sets, vector<Rule> &rules, vector<FirstSet> &firstSets, vector<Channel> &channels, int &accept, Token start)
{
	int i,j,n;
	LRSet tempset;
	PCRule temppc;
	ShiftLink templink;
	Token temptoken;
	bool setfound;
	int r;
	vector<Channel> tempchannels1;
	vector<Channel> tempchannels2;

	sets.push_back(LRSet());

	for(i=0;i<rules.size();i++)
		if(rules[i].leftToken()==start) 
			sets[0].addPCRule(PCRule(rules[i],i,vector<Token>()));

	sets[0].populateSet(rules, firstSets, channels);

	for(i=0;i<channels.size();i++) channels[i].in.set=channels[i].out.set=0;

	for(n=0;n<sets.size();n++)
	{
		for(i=0;i<sets[n].numRules();i++)
		{
			if(sets[n][i].complete()) continue;
			
			tempchannels1.clear();
			tempchannels2.clear();
			tempset.clear();
			temptoken=sets[n][i].nextToken();
	
			if(sets[n].isLink(temptoken)==false)
			{
				for(j=i;j<sets[n].numRules();j++)
				{
					if(sets[n][j].complete()) continue;
					if(sets[n][j].nextToken()==temptoken)
					{
						temppc=sets[n][j];
						temppc++;
						tempset.addPCRule(temppc);
						tempchannels1.push_back(Channel(n,j,-1,tempset.numRules()-1,0));
					}
				}
				tempset.populateSet(rules, firstSets, tempchannels2);
				templink.token=temptoken;

				setfound=false;
				for(j=0;j<sets.size();j++) 
				{
					if(tempset==sets[j])
					{
						templink.shiftState=j;
						setfound=true;
					}
				}
				
				if(setfound==true)
				{
					for(j=0;j<tempchannels1.size();j++)
					{
						r=sets[templink.shiftState].findRule(tempset[tempchannels1[j].out.rule]);
						tempchannels1[j].out.set=templink.shiftState;;
						tempchannels1[j].out.rule=r;
						channels.push_back(tempchannels1[j]);
					}
				}

				if(setfound==false)
				{
					templink.shiftState=sets.size();
					for(j=0;j<tempchannels1.size();j++)
					{
						tempchannels1[j].out.set=templink.shiftState;
						channels.push_back(tempchannels1[j]);
					}
					for(j=0;j<tempchannels2.size();j++)
					{
						tempchannels2[j].in.set=templink.shiftState;
						tempchannels2[j].out.set=templink.shiftState;
						channels.push_back(tempchannels2[j]);
					}
					sets.push_back(tempset);
					for(j=0;j<tempset.numRules();j++)
						if(tempset[j].leftToken()==start && tempset[j].complete()==true) accept=templink.shiftState;
				}

				sets[n].addLink(templink);
			}
		}
	}
}

void generateFirstSets(vector<FirstSet> &firstSets, vector<Rule> &rules)
{
	int i;
	bool changedSets;

	do
	{
		changedSets=false;
		for(i=0;i<rules.size();i++)
		{
			if(rules[i][0].isTerminal())
			{
				if(firstSets[rules[i].leftToken().num()].add(rules[i][0])) changedSets=true;
			}
			else
			{
				if(firstSets[rules[i].leftToken().num()].add(firstSets[rules[i][0].num()])) changedSets=true;
			}
		}
	} while(changedSets);
}

void generateStateTable(StateTable &stateTable, vector<LRSet> &sets, vector<Token> &tokens)
{
	int i,j,k;
	Token temptoken;
	
	for(i=0;i<sets.size(); i++)
	{
		for(j=0;j<tokens.size();j++)
		{
			temptoken=tokens[j];
			
			stateTable[i][j].action=UNDEF;
			stateTable[i][j].number=0;
			stateTable[i][j].ruleSize=0;
			stateTable[i][j].newToken=0;
			
			if(sets[i].isLink(temptoken))
			{
				if(stateTable[i][j].action!=UNDEF) cout << "Error" << endl;
				stateTable[i][j].action=SHIFT;
				stateTable[i][j].number=sets[i][temptoken].shiftState;
			}

			for(k=0;k<sets[i].numRules();k++)
			{
				if(sets[i][k].complete()==false) continue;
				if(sets[i][k].hasLookahead(temptoken))
				{
					switch(stateTable[i][j].action)
					{
					case SHIFT:
						cout << "Warning: Shift/Reduce Conflict on \'" << temptoken << "\'" << endl << endl << sets[i] << endl;
						break;
					case REDUCE:
						cout << "Warning: Reduce/Reduce Conflict on \'" << temptoken << "\'" << endl << endl << sets[i] << endl;
						break;
					case UNDEF:
						stateTable[i][j].action=REDUCE;
						stateTable[i][j].number=sets[i][k].ruleNum();
						stateTable[i][j].ruleSize=sets[i][k].length();
						stateTable[i][j].newToken=sets[i][k].leftToken().getType();
					}
				}
			}
		}
	}
}


void generateParser(vector<Rule> &rules, vector<Token> &tokens, Token start)
{
	vector<Token> lookahead;
	vector<Channel> channels;
		
	int accept;

	vector<FirstSet> firstSets;
	bool changed;
	vector<LRSet> sets;
	int numStates;
	int i;
	PCRule temprule;
	Token laToken;
	int j;

	for(i=0;i<tokens.size();i++) firstSets.push_back(FirstSet());

	generateFirstSets(firstSets, rules);
	
	generateSets(sets, rules, firstSets, channels, accept, start);

	changed=true;
	while(changed)
	{
		changed=false;
		for(i=0;i<channels.size();i++)
		{
			lookahead.clear();
			temprule=sets[channels[i].in.set][channels[i].in.rule];
			if(temprule.nextTokenIndex()==temprule.length()-1 || channels[i].type==0) 
			{
				lookahead=temprule.lookAhead();
				if(sets[channels[i].out.set][channels[i].out.rule].addLookahead(lookahead)) changed=true;
			}
			else
			{
				laToken=temprule.twoNext();
				
				if(laToken.isTerminal()) 
				{
					if(sets[channels[i].out.set][channels[i].out.rule].addLookahead(laToken)) changed=true;
				}
				else
				{
					if(sets[channels[i].out.set][channels[i].out.rule].addLookahead(firstSets[laToken.num()])) changed=true;
					//cout << "Adding FirstSet of " << laToken << " to  " << channels[i].in.set << ": " << temprule << " to " << channels[i].out.set << ": " << sets[channels[i].out.set][channels[i].out.rule] << endl;

				}
			}
			
		}
	}

//	for(i=0;i<channels.size();i++) cout << channels[i].in.set << ":" << sets[channels[i].in.set][channels[i].in.rule] << " -> " << channels[i].out.set << ": " << sets[channels[i].out.set][channels[i].out.rule] << endl;
	numStates=sets.size();
	StateTable stateTable(numStates, tokens.size(), accept);

	generateStateTable(stateTable, sets, tokens);
	//stateTable.Print(tokens);

	stateTable.Save("..\\Engine\\C_StateTable.inc");
	//for(i=0;i<sets.size();i++) cout << "Set " << i << ":" << endl << sets[i] << endl;
	
//	cout << "Accept: " << accept << endl;
//	for(i=0;i<numTokens;i++) cout << "FirstSet " << tokens[i] << ": " << firstSets[i] << endl;
//	printStateTable(stateTable, numStates, numTokens);
}

void main()
{
	string tempString;
	ifstream grammarFile;
	ofstream tokenizerOut;
	ofstream actionsOut;
	int i;
	int numTerminals;
	
	numTerminals=0;
	grammarFile.open("grammar.txt");
	actionsOut.open("..\\Engine\\C_Actions.inc");

	i=0;

	actionsOut << "switch(number)" << endl << "{" << endl;

	while(!grammarFile.eof())
	{
		getline(grammarFile, tempString);

		if(tempString[0]=='%')
		{
			parseRule(tempString.substr(1));
			if(i!=0) actionsOut << "break;" << endl << endl;
			actionsOut << "case " << i << ":" << endl;
			i++;
		}
		else if(tempString!="") actionsOut << tempString << endl;
	}

	actionsOut << "}" << endl;

	generateParser(rules, tokens, FindOrAddToken("S'", false));

	tokenizerOut.open("..\\Engine\\C_Tokenizer.inc");
	tokenizerOut << "C_Terminal C_Tokenizer::terminals[] = {" << endl;
	for(i=0;i<tokens.size();i++)
	{
		if(tokens[i].isTerminal()) 
		{
			tokenizerOut << "\"" << tokens[i].getName() << "\", " << i << "," << endl;
			numTerminals++;
		}
	}
	tokenizerOut << "};" << endl << endl;
	tokenizerOut << "int C_Tokenizer::numTerminals = " << numTerminals << ";" << endl;
	
}