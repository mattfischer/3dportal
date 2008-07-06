#include <string>

#include "M_Vector.h"

#include "C_Tokenizer.h"

#include "C_Tokenizer.inc"

C_Tokenizer::C_Tokenizer( const string& source )
{
	int i;

	p = 0;
	eof = false;
	lineNum = 1;
	data = source;

	for( i = 0 ; i < numTerminals ; i++ )
	{
		if( terminals[i].name == "<id>" )      idIndex = terminals[i].index;
		if( terminals[i].name == "<end>" )     endIndex = terminals[i].index;
		if( terminals[i].name == "<float>" )   floatIndex = terminals[i].index;
		if( terminals[i].name == "<int>" )     intIndex = terminals[i].index;
		if( terminals[i].name == "<message>" ) messageIndex = terminals[i].index;
		if( terminals[i].name == "<hex>" )     hexIndex = terminals[i].index;
		if( terminals[i].name == "<vector>" )  vectorIndex = terminals[i].index;
		if( terminals[i].name == "<string>" )  stringIndex = terminals[i].index;
	}
}

C_Token C_Tokenizer::NextToken(bool stringHack, bool stringHack2)
{
	std::string a, b;
	int i, j;
	int result=-1;
	C_Token newToken;
	bool integer;
	bool found=false;
	int maxLength=0;
	bool repeat;
	bool notNumber;
	char buffer[100];
	M_Vector newVector;

	if(lineNum==490)
		repeat=true;

	repeat=true;
	while(repeat)
	{
		repeat=false;
		while(p<data.length() && (data[p]=='#' || (data[p]=='/' && data[p+1]=='/')))
		{
			repeat=true;
			while(p<data.length() && data[p]!='\n') p++;
			p++;
			lineNum++;
		}

		while(p<data.length() && (data[p]==' ' || data[p]=='\t' || data[p]=='\n' || data[p]=='\r'))
		{
			repeat=true;
			if(data[p]=='\n') lineNum++;
			p++;
		}
	}

	if(p>=data.length()) eof=true;

	if(eof==true)
	{
		newToken.type=endIndex;
		newToken.line=lineNum;
		return newToken;
	}

	if(data[p]=='0' && data[p+1]=='x')
	{
		i=p;
		while(data[i]=='x' || (data[i]>='0' && data[i]<='9') || 
			(data[i]>='A' && data[i]<='F') || 
			(data[i]>='a' && data[i]<='f')) i++;
		newToken.type=hexIndex;
		newToken.lexData=new int;
		*(int*)newToken.lexData=ParseHex(data.substr(p, i-p).c_str());
		found=true;
		p=i;
	}
	
	notNumber=false;
	if(data[p]>='0' && data[p]<='9')
	{
		i=p;
		while(data[i]>='0' && data[i]<='9') i++;
		if(data[i]=='_' || (data[i]>='a' && data[i]<='z') || (data[i]>='A' && data[i]<='Z'))
			notNumber=true;
	}

	if(found==false && !notNumber)
	{
		integer=true;
		i=p;
		while(data[i]=='.' || (data[i]>='0' && data[i]<='9') || (stringHack && data[i]=='-'))
		{
			if(data[i]=='.')
			{
				if(data[i+1]>='0' && data[i+1]<='9') integer=false;
				else break;
			}
			i++;
		}

		if(i!=p)
		{
			if(integer)
			{
				newToken.type=intIndex;
				newToken.lexData=(void*)new int;
				*((int*)newToken.lexData)=atoi(data.substr(p, i-p).c_str());
			}
			else
			{
				newToken.type=floatIndex;
				newToken.lexData=(void*)new float;
				*((float*)newToken.lexData)=atof(data.substr(p, i-p).c_str());
			}
			p=i;
			found=true;
		}
	}

	if(found==false)
	{
		if(data[p]=='\'')
		{
			i=p+1;
			while(data[i]!=' ') i++;
			newToken.type=vectorIndex;
			strcpy(buffer, data.substr(p+1, i-p-2).c_str());
			newVector.x=atof(buffer);

			p=i;
			i++;
			while(data[i]!=' ') i++;
			newToken.type=vectorIndex;
			strcpy(buffer, data.substr(p+1, i-p-2).c_str());
			newVector.y=atof(buffer);

			p=i;
			i++;
			while(data[i]!='\'') i++;
			newToken.type=vectorIndex;
			strcpy(buffer, data.substr(p+1, i-p-2).c_str());
			newVector.z=atof(buffer);

			newToken.lexData=new M_Vector(newVector);
			
			found=true;
			p=i+1;
		}
	}

	if(found==false)
	{
		if(data[p]=='\"')
		{
			i=p+1;
			while(data[i]!='\"') i++;
			newToken.type=stringIndex;
			newToken.lexData=new char[i-p-1];
			strcpy((char*)newToken.lexData, data.substr(p+1, i-p-2).c_str());
			found=true;
			p=i+1;
		}
	}

	if(found==false)
	{
		for(j=0;j<numTerminals;j++)
		{
			a=terminals[j].name;
			b=data.substr(p, a.length());
			
			for(i=0;i<b.length();i++)
				if(b[i]>='A' && b[i]<='Z') b[i]=b[i]-'A'+'a';

			if(a==b && a.length()>maxLength)
			{
				found=true;
				newToken.type=terminals[j].index;
				maxLength=a.length();
			}
		}
	
		j=p;
		while(j<data.length() && (
		(data[j]>='a' && data[j]<='z') || 
		(data[j]>='A' && data[j]<='Z') ||
		(data[j]>='0' && data[j]<='9') ||
		data[j]=='_' || data[j]=='.' || (stringHack && (data[j]=='-' || data[j]=='+'))
		|| (stringHack2 && data[j]==' '))) j++;
		a=data.substr(p, j-p);
		

		if(a.length()>maxLength)
		{
			for(i=0;i<a.length();i++)
				if(a[i]>='A' && a[i]<='Z') a[i]=a[i]-'A'+'a';

			newToken.type=idIndex;
			newToken.lexData=new char[a.length()+1];
			strcpy((char*)newToken.lexData, a.c_str());
			p=j;
		}
		else p+=maxLength;
	}
	
	newToken.line=lineNum;

	return newToken;
}

int C_Tokenizer::ParseHex(const char *string)
{
	int retVal;
	int placeVal;
	int i;
	
	retVal=0;
	placeVal=1;
	for(i=strlen(string)-1;i>=2;i--)
	{
		if(string[i]>='0' && string[i]<='9')
			retVal+=placeVal*(string[i]-'0');
		else if(string[i]>='A' && string[i]<='F')
			retVal+=placeVal*(string[i]-'A'+10);
		else if(string[i]>='a' && string[i]<='f')
			retVal+=placeVal*(string[i]-'a'+10);

		placeVal*=16;
	}
	return retVal;
}
	
