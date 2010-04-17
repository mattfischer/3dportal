%{
	#include "C_AST.h"
	#include <malloc.h>
	#include <stdarg.h>
	
	static C_ASTNode *NewNode(C_AST_NodeType type, ...);
	static C_ASTNode *AddChildren(C_ASTNode *node1, C_ASTNode *node2);
	static C_ASTNode *AddNode(C_ASTNode *node1, C_ASTNode *node2);
	static char *copyString(char *string);
		
	C_ASTNode *C_ParseTree;
%}



%union {
	char *stringVal;
	int intVal;
	float floatVal;
	C_AST_Vector vectorVal;
	C_ASTNode *astNode;
}

%expect 1

%token <intVal> HEX
%token <vectorVal> VECTOR
%token <stringVal> STRING
%token <floatVal> FLOAT
%token <intVal> INT
%token <stringVal> ID
%token END_FILE
%token LAND
%token LOR
%token EQUALS
%token NEQUALS
%token GEQUALS
%token LEQUALS
%token FLAGS
%token DESC
%token LOCAL
%token NOLINK
%token LINKID
%token CALL
%token RETURN
%token STOP
%token IF
%token ELSE
%token FOR
%token WHILE
%token MASK
%token CODE
%token SYMBOLS_
%token END


%type <astNode> start cog_file symbol_section code_section symbol_list symbol_entry
%type <astNode> name symbol_use_list literal symbol_use statement_list statement
%type <astNode> verb_call expression argument_list assignment_expression log_or_expression
%type <astNode> log_and_expression bool_expression bool_op arithmetic_expression
%type <astNode> plus_expression multiply_expression bin_or_expression bin_and_expression
%type <astNode> base_expression 
%%
start: cog_file END_FILE
{
	C_ParseTree = $1;
}

cog_file: FLAGS '=' HEX symbol_section code_section
{
	$$ = NewNode(NODE_COG_FLAGS, $4, $5, NULL);
	$$->lexData.intVal = $3;
}

cog_file: symbol_section code_section
{
	$$ = NewNode(NODE_COG, $1, $2, NULL);
}

symbol_section: SYMBOLS_ symbol_list END
{
	$$ = $2;
}

symbol_list: symbol_entry
{
	$$ = NewNode(NODE_SYMBOL_LIST, $1, NULL);
}

symbol_list: symbol_list symbol_entry
{
	$$ = AddNode($1, $2);
}


symbol_entry: ID name symbol_use_list
{
	$$ = NewNode(NODE_SYMBOL_ENTRY_USE, $2, $3, NULL);
	$$->lexData.stringVal = $1;
}

symbol_entry: ID name
{
	$$ = NewNode(NODE_SYMBOL_ENTRY, $2, NULL);
	$$->lexData.stringVal = $1;
}


name: ID
{
	$$ = NewNode(NODE_NAME, NULL);
	$$->lexData.stringVal = $1;
}

name: ID '=' literal
{
	$$ = NewNode(NODE_NAME_INIT, $3, NULL);
	$$->lexData.stringVal = $1;
}

name: FLAGS
{
	$$ = NewNode(NODE_NAME, NULL);
	$$->lexData.stringVal = copyString("flags");
}

name: FLAGS '=' literal
{
	$$ = NewNode(NODE_NAME_INIT, $3, NULL);
	$$->lexData.stringVal = copyString("flags");
}

symbol_use_list: symbol_use
{
	$$ = NewNode(NODE_SYMBOL_USE_LIST, $1, NULL);
}

symbol_use_list: symbol_use_list symbol_use
{
	$$ = AddNode($1, $2);
}

symbol_use_list: symbol_use_list ',' symbol_use
{
	$$ = AddNode($1, $3);
}

symbol_use: DESC '=' ID
{
	$$ = NewNode(NODE_SYMBOL_USE_DESC, NULL);
}

symbol_use: LOCAL
{
	$$ = NewNode(NODE_SYMBOL_USE_LOCAL, NULL);
}

symbol_use: NOLINK
{
	$$ = NewNode(NODE_SYMBOL_USE_NOLINK, NULL);
}

symbol_use: LINKID '=' literal
{
	$$ = NewNode(NODE_SYMBOL_USE_LINKID, $3, NULL);
}

symbol_use: MASK '=' literal
{
	$$ = NewNode(NODE_SYMBOL_USE_MASK, $3, NULL);
}

code_section: CODE statement_list END
{
	$$ = $2;
}

code_section: CODE statement_list
{
	$$ = $2;
}


statement_list: statement
{
	$$ = NewNode(NODE_STATEMENT_LIST, $1, NULL);
}

statement_list: statement_list statement
{
	$$ = AddNode($1, $2);
}

statement: ID ';'
{
	$$ = NewNode(NODE_STATEMENT_EMPTY, NULL);
}

statement: '{' statement_list '}'
{
	$$ = $2;
}

statement: ID ':'
{
	$$ = NewNode(NODE_STATEMENT_LABEL, NULL);
	$$->lexData.stringVal = $1;
}

statement: verb_call ';'
{
	$$ = $1;
}

statement: CALL ID ';'
{
	$$ = NewNode(NODE_STATEMENT_CALL, NULL);
	$$->lexData.stringVal = $2;
}

statement: ID '=' expression ';'
{
	$$ = NewNode(NODE_STATEMENT_ASSIGN, $3, NULL);
	$$->lexData.stringVal = $1;
}

statement: ID '[' expression ']' '=' expression ';'
{
	$$ = NewNode(NODE_STATEMENT_ASSIGN_INDEXED, $3, $6, NULL);
	$$->lexData.stringVal = $1;
}

statement: FLAGS '=' expression ';'
{
	$$ = NewNode(NODE_STATEMENT_ASSIGN, $3, NULL);
	$$->lexData.stringVal = copyString("flags");
}

statement: '{' '}'
{
	$$ = NewNode(NODE_STATEMENT_EMPTY, NULL);
}

statement: RETURN ';'
{
	$$ = NewNode(NODE_STATEMENT_RETURN, NULL);
}

statement: STOP ';'
{
	$$ = NewNode(NODE_STATEMENT_RETURN, NULL);
}

statement: IF '(' expression ')' statement
{
	$$ = NewNode(NODE_STATEMENT_IF, $3, $5, NULL);
}

statement: IF '(' expression ')' ';' statement
{
	$$ = NewNode(NODE_STATEMENT_IF, $3, $6, NULL);
}

statement: IF '(' expression ')' statement ELSE statement
{
	$$ = NewNode(NODE_STATEMENT_IF_ELSE, $3, $5, $7, NULL);
}

statement: FOR '(' expression ';' expression ';' expression ')' statement
{
	$$ = NewNode(NODE_STATEMENT_FOR, $3, $5, $7, $9, NULL);
}

statement: WHILE '(' expression ')' statement
{
	$$ = NewNode(NODE_STATEMENT_WHILE, $3, $5, NULL);
}

verb_call: ID '(' ')'
{
	$$ = NewNode(NODE_STATEMENT_VERBCALL, NULL);
	$$->lexData.stringVal = $1;
}

verb_call: ID '(' argument_list ')'
{
	$$ = AddChildren(NewNode(NODE_STATEMENT_VERBCALL, NULL), $3);
	$$->lexData.stringVal = $1;
}


argument_list: expression
{
	$$ = NewNode(NODE_ARGUMENT_LIST, $1, NULL);
}

argument_list: argument_list ',' expression
{
	$$ = AddNode($1, $3);
}

expression: ID '=' assignment_expression
{
	$$ = NewNode(NODE_EXPRESSION_ASSIGN, $3, NULL);
	$$->lexData.stringVal = $1;
}

expression: ID '[' expression ']' '=' expression ';'
{
	$$ = NewNode(NODE_EXPRESSION_ASSIGN_INDEXED, $3, $6, NULL);
	$$->lexData.stringVal = $1;
}

expression: assignment_expression
{
	$$ = $1;
}


assignment_expression: assignment_expression LOR log_or_expression
{
	$$ = NewNode(NODE_EXPRESSION_LOR, $1, $3, NULL);
}

assignment_expression: log_or_expression
{
	$$ = $1;
}


log_or_expression: log_or_expression LAND log_and_expression
{
	$$ = NewNode(NODE_EXPRESSION_LAND, $1, $3, NULL);
}

log_or_expression: log_and_expression
{
	$$ = $1;
}


log_and_expression: '!' bool_expression
{
	$$ = NewNode(NODE_EXPRESSION_LINVERSE, $2, NULL);
}

log_and_expression: bool_expression
{
	$$ = $1;
}


bool_expression: arithmetic_expression bool_op arithmetic_expression
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN, $1, $2, $3, NULL);
}

bool_expression: arithmetic_expression
{
	$$ = $1;
}


bool_op: EQUALS
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_EQUAL, NULL);
}

bool_op: NEQUALS
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_UNEQUAL, NULL);
}

bool_op: '>'
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_GREATER, NULL);
}

bool_op: '<'
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_LESS, NULL);
}

bool_op: GEQUALS
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_GREATER_EQUAL, NULL);
}

bool_op: LEQUALS
{
	$$ = NewNode(NODE_EXPRESSION_BOOLEAN_LESS_EQUAL, NULL);
}

arithmetic_expression: arithmetic_expression '+' plus_expression
{
	$$ = NewNode(NODE_EXPRESSION_PLUS, $1, $3, NULL);
}

arithmetic_expression: arithmetic_expression '-' plus_expression
{
	$$ = NewNode(NODE_EXPRESSION_MINUS, $1, $3, NULL);
}

arithmetic_expression: plus_expression
{
	$$ = $1;
}


plus_expression: plus_expression '*' multiply_expression
{
	$$ = NewNode(NODE_EXPRESSION_TIMES, $1, $3, NULL);
}

plus_expression: plus_expression '/' multiply_expression
{
	$$ = NewNode(NODE_EXPRESSION_DIVIDE, $1, $3, NULL);
}

plus_expression: plus_expression '%' multiply_expression
{
	$$ = NewNode(NODE_EXPRESSION_MODULUS, $1, $3, NULL);
}

plus_expression: multiply_expression
{
	$$ = $1;
}


multiply_expression: multiply_expression '|' bin_or_expression
{
	$$ = NewNode(NODE_EXPRESSION_BOR, $1, $3, NULL);
}

multiply_expression: bin_or_expression
{
	$$ = $1;
}


bin_or_expression: bin_or_expression '&' bin_and_expression
{
	$$ = NewNode(NODE_EXPRESSION_BAND, $1, $3, NULL);
}

bin_or_expression: bin_and_expression
{
	$$ = $1;
}


bin_and_expression: '-' base_expression
{
	$$ = NewNode(NODE_EXPRESSION_NEGATIVE, $2, NULL);
}

bin_and_expression: base_expression
{
	$$ = $1;
}


base_expression: '(' expression ')'
{
	$$ = $2;
}

base_expression: literal
{
	$$ = $1;
}

base_expression: verb_call
{
	$$ = $1; 
}

base_expression: ID '[' expression ']'
{
	$$ = NewNode(NODE_EXPRESSION_INDEX, $3, NULL);
	$$->lexData.stringVal = $1;
}

literal: ID
{
	$$ = NewNode(NODE_ID, NULL);
	$$->lexData.stringVal = $1;
}

literal: FLAGS
{
	$$ = NewNode(NODE_ID, NULL);
	$$->lexData.stringVal = copyString("flags");
}

literal: INT
{
	$$ = NewNode(NODE_INT, NULL);
	$$->lexData.intVal = $1;
}

literal: FLOAT
{
	$$ = NewNode(NODE_FLOAT, NULL);
	$$->lexData.floatVal = $1;
}

literal: HEX
{
	$$ = NewNode(NODE_HEX, NULL);
	$$->lexData.intVal = $1;
}

literal: VECTOR
{
	$$ = NewNode(NODE_VECTOR, NULL);
	$$->lexData.vectorVal = $1;
}

literal: STRING
{
	$$ = NewNode(NODE_STRING, NULL);
	$$->lexData.stringVal = $1;
}

literal: LINKID
{
	$$ = NewNode(NODE_LINKID, NULL);
}
%%  

int yyerror(char *string)
{
return 0;
}

static C_ASTNode *NewNode(C_AST_NodeType type, ...)
{
    va_list list;
    int i;
	int numArgs = 0;
	C_ASTNode *astnode;
	C_ASTNode *temp;	
	
	va_start(list, type);
	while(va_arg(list, C_ASTNode*)) numArgs++;
	    
    va_start(list, type);
    
    astnode = (C_ASTNode*)malloc(sizeof(C_ASTNode));
    astnode->nodeType = type;
    astnode->children = (C_ASTNode**)malloc(sizeof(C_ASTNode*) * numArgs);
    astnode->numChildren = numArgs;
    for(i = 0; i < numArgs; i++)
	    astnode->children[i] = va_arg(list, C_ASTNode*);
	
    return astnode;
}

static C_ASTNode *AddChildren(C_ASTNode *node1, C_ASTNode *node2)
{
    int i;
    C_ASTNode **tempAST = (C_ASTNode**)malloc(sizeof(C_ASTNode*) * (node1->numChildren + node2->numChildren));

    for(i = 0; i < node1->numChildren; i++) 
	    tempAST[i] = node1->children[i];
    for(i = 0; i < node2->numChildren; i++) 
	    tempAST[i + node1->numChildren] = node2->children[i];
	
    if(node1->numChildren != 0) 
	    free(node1->children);
    node1->children = tempAST;
    node1->numChildren += node2->numChildren;

    free(node2->children);
    free(node2);

    return node1;
}

static C_ASTNode *AddNode(C_ASTNode *node1, C_ASTNode *node2)
{
    int i;
    int numChildren = node1->numChildren;	
    C_ASTNode **tempAST = (C_ASTNode**)malloc(sizeof(C_ASTNode*) * (numChildren + 1));

    for(i = 0; i < numChildren; i++) 
	    tempAST[i] = node1->children[i];
    tempAST[numChildren] = node2;

    if(node1->numChildren != 0) 
	    free(node1->children);
    node1->children = tempAST;
    node1->numChildren++;
	
    return node1;
}

static char *copyString(char *string)
{
	char *result = (char*)malloc(strlen(string) + 1);
	strcpy(result, string);
	return result;
}