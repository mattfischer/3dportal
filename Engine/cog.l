%option noyywrap

%s SYMBOLS

%{
#include <math.h>

#include "C_ASTNode.h"

C_AST_LexData yylval;
int linenum;

void parseVector(char *, float*, float*, float*);
char *copyString(char *, int);
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

{HEX} yylval.intVal = parseHex(yytext); return 0;
{VECTOR} parseVector(yytext, &yylval.vectorVal.x, &yylval.vectorVal.y, &yylval.vectorVal.z); return 1;
{STRING} yylval.stringVal = copyString(yytext + 1, 1); return 2;
{FLOAT} yylval.floatVal = atof(yytext); return 3;
{INT} yylval.intVal = atoi(yytext); return 4;

(=|,|;|\{|\}|:|\[|\]|\(|\)|&&|\|\|) yylval.stringVal = yytext; return 5;
(!|==|!=|>|<|>=|<=|\+|-|\*|\/|%|&|\|) yylval.stringVal = yytext; return 5;

symbols BEGIN(SYMBOLS); yylval.stringVal = yytext; return 6;
end BEGIN(INITIAL); yylval.stringVal = yytext; return 6;

(flags|symbols|end|desc|local|nolink|linkid) yylval.stringVal = yytext; return 6;
(call|return|stop|if|else|for|while|mask|code) yylval.stringVal = yytext; return 6;

<SYMBOLS>-{INT} yylval.intVal = atoi(yytext); return 4;
<SYMBOLS>[[:alnum:]_.+\-]+ yylval.stringVal = copyString(yytext, 0); return 7;

[[:alnum:]_\.]+ yylval.stringVal = copyString(yytext, 0); return 7;

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

char *copyString(char *string, int endchop)
{
	int len = strlen(string);
	char *result = (char*)malloc(len + 1 - endchop);
	strncpy(result, string, len - endchop);
	result[len - endchop] = '\0';
	return result;
} 