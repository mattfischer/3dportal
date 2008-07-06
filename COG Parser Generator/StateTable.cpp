#include "StateTable.h"

#include <fstream>

using namespace std;

StateTable::StateTable(int p_numStates, int p_numTokens, int p_accept)
{
	int i;

	accept=p_accept;
	numStates=p_numStates;
	numTokens=p_numTokens;
	table=new StateTableItem[numStates*numTokens];
}

StateTable::StateTable(char *filename)
{
	FILE *file;
	int i;

	file=fopen(filename, "rb");

	fread(&numTokens, sizeof(int), 1, file);
	fread(&numStates, sizeof(int), 1, file);
	fread(&accept, sizeof(int), 1, file);

	table=new StateTableItem[numStates*numTokens];
	for(i=0;i<numStates*numTokens;i++)
		fread(&table[i], sizeof(StateTableItem), 1, file);

	fclose(file);
}

StateTable::~StateTable()
{
	delete [] table;
}

StateTableItem *StateTable::operator[](int n)
{
	return table+n*numTokens;
}

void StateTable::Print(vector<Token> &tokens)
{
	int i,j;
	
	cout << " ";
	for(i=0;i<numTokens;i++) cout << "   " << tokens[i];
		
	cout << endl << endl;
	
	for(i=0;i<numStates;i++)
	{
		cout << setw(2) << i << ": ";
		for(j=0;j<numTokens;j++) 
		{
			if(table[i*numTokens+j].action==UNDEF) cout << "    ";
			else
			{
				if(table[i*numTokens+j].action==SHIFT) cout << "s";
				if(table[i*numTokens+j].action==REDUCE) cout << "r";
				cout << setw(2) << table[i*numTokens+j].number << " ";
			}
		}
		cout << endl;
	}
}

void StateTable::Save(char *filename)
{
	ofstream file;
	int i, j;
	StateTableItem item;

	file.open(filename);

	file << "int C_StateTable::numTokens=" << numTokens << ";" << endl;
	file << "int C_StateTable::numStates=" << numStates << ";" << endl;
	file << "int C_StateTable::accept=" << accept << ";" << endl;
	file << endl;
	file << "C_StateTableItem C_StateTable::table[]={" << endl;

	for(i=0;i<numStates;i++)
	{
		for(j=0;j<numTokens;j++)
		{
			item=table[i*numTokens+j];	
			switch(item.action)
			{
			case UNDEF: file << "UNDEF"; break;
			case SHIFT: file << "SHIFT"; break;
			case REDUCE: file << "REDUCE"; break;
			}
			file << ", " << item.number << ", " << item.ruleSize << ", " << item.newToken << "," << endl;
		}
		file << endl;
	}
	
	file << "};";
}