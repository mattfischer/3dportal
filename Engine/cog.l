%option noyywrap

%s SYMBOLS

%{
#include <math.h>

#include "C_ASTNode.h"
#include "cog.tab.h"

int linenum;

void parseVector(char *, C_AST_Vector *vector);
char *copyString(char *, int);
int findToken(char *token_buffer);
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

{HEX} yylval.intVal = parseHex(yytext); return HEX;
{VECTOR} parseVector(yytext, &yylval.vectorVal); return VECTOR;
{STRING} yylval.stringVal = copyString(yytext + 1, 1); return STRING;
{FLOAT} yylval.floatVal = atof(yytext); return FLOAT;
{INT} yylval.intVal = atoi(yytext); return INT;

(=|,|;|\{|\}|:|\[|\]|\(|\)) return *yytext;
(!|>|<|\+|-|\*|\/|%|&|\|) return *yytext;

&&		return LAND;
\|\|	return LOR;
==		return EQUALS;
!=		return NEQUALS;
>=		return GEQUALS;
\<=		return LEQUALS;
flags	return FLAGS;
desc	return DESC;
local	return LOCAL;
nolink	return NOLINK;
linkid	return LINKID;
call	return CALL;
return	return RETURN;
stop	return STOP;
if		return IF;
else	return ELSE;
for		return FOR;
while	return WHILE;
mask	return MASK;
code	return CODE;
symbols BEGIN(SYMBOLS); return SYMBOLS_;
end BEGIN(INITIAL); return END;

<SYMBOLS>-{INT} yylval.intVal = atoi(yytext); return INT;
<SYMBOLS>[[:alnum:]_.+\-]+ yylval.stringVal = copyString(yytext, 0); return ID;

[[:alnum:]_\.]+ yylval.stringVal = copyString(yytext, 0); return ID;

<<EOF>> return END_FILE;

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

void parseVector(char *string, C_AST_Vector *vector)
{
	int start;
	int end;
	
	start = 1;
	for(end = start; string[end] != ' '; end++) ;
	string[end] = '\0';
	vector->x = atof(string + start);
	string[end] = ' ';
	start = end + 1;
	
	for(end = start; string[end] != ' '; end++) ;
	string[end] = '\0';
	vector->y = atof(string + start);
	string[end] = ' ';
	start = end + 1;
	
	for(end = start; string[end] != '\''; end++) ;
	string[end] = '\0';
	vector->z = atof(string + start);
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
