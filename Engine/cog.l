%option noyywrap

%s SYMBOLS

%{
#include <math.h>

int intval;
float floatval;
char *stringval;
float vecx;
float vecy;
float vecz;
int linenum;

void parseVector(char *, float*, float*, float*);
%}

HEX 0x[[:xdigit:]]+
INT [[:digit:]]+
FLOAT [[:digit:]]*\.[[:digit:]]+
NUMBER ({FLOAT}|{INT})
VECTOR \'-?{NUMBER}\ -?{NUMBER}\ -?{NUMBER}\'
STRING \"[^\"]*\"
%%
#.* 
\/\/.*
[[:blank:]] 

\r\n linenum++;

{HEX} intval = parseHex(yytext); return 0;
{VECTOR} parseVector(yytext, &vecx, &vecy, &vecz); return 1;
{STRING} stringval = yytext; return 2;
{FLOAT} floatval = atof(yytext); return 3;
{INT} intval = atoi(yytext); return 4;

(=|,|;|\{|\}|:|\[|\]|\(|\)|&&|\|\|) stringval = yytext; return 5;
(!|==|!=|>|<|>=|<=|\+|-|\*|\/|%|&|\|) stringval = yytext; return 5;

symbols BEGIN(SYMBOLS); stringval = yytext; return 6;
end BEGIN(INITIAL); stringval = yytext; return 6;

(flags|symbols|end|desc|local|nolink|linkid) stringval = yytext; return 6;
(call|return|stop|if|else|for|while|mask|code) stringval = yytext; return 6;

<SYMBOLS>-{INT} intval = atoi(yytext); return 4;
<SYMBOLS>[[:alnum:]_.+\-]+ stringval = yytext; return 7;

[[:alnum:]_\.]+ stringval = yytext; return 7;

<<EOF>> return 8;

%%

int parseHex(char *string)
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

void parseVector(char *string, float *vecx, float *vecy, float *vecz)
{
	int start;
	int end;
	
	start = 1;
	for(end = start; string[end] != ' '; end++) ;
	string[end] = '\0';
	*vecx = atof(string + start);
	string[end] = ' ';
	start = end + 1;
	
	for(end = start; string[end] != ' '; end++) ;
	string[end] = '\0';
	*vecy = atof(string + start);
	string[end] = ' ';
	start = end + 1;
	
	for(end = start; string[end] != '\''; end++) ;
	string[end] = '\0';
	*vecz = atof(string + start);
	string[end] = '\'';
	start = end + 1;
}