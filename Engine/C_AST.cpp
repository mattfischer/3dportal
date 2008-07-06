#include <stdarg.h>

#include "C_Tokenizer.h"
#include "C_AST.h"

C_ASTNode *NewNode(NodeType type, int numArgs, ...)
{
	va_list list;
	int i;

	va_start(list, numArgs);
	C_ASTNode *astnode;
	astnode=new C_ASTNode;
	astnode->nodeType=type;
	astnode->children=new C_ASTNode*[numArgs];
	astnode->numChildren=numArgs;
	for(i=0;i<numArgs;i++)
		astnode->children[i]=va_arg(list, C_ASTNode*);
	
	return astnode;
}

C_ASTNode *AddChildren(C_ASTNode *node1, C_ASTNode *node2)
{
	int i;
	C_ASTNode **tempAST=new C_ASTNode*[node1->numChildren+node2->numChildren];

	for(i=0;i<node1->numChildren;i++) 
		tempAST[i]=node1->children[i];
	for(i=0;i<node2->numChildren;i++) 
		tempAST[i+node1->numChildren]=node2->children[i];
	
	if(node1->numChildren!=0) 
		delete [] node1->children;
	node1->children=tempAST;
	node1->numChildren+=node2->numChildren;

	delete [] node2->children;
	delete node2;

	return node1;
}

C_ASTNode *AddNode(C_ASTNode *node1, C_ASTNode *node2)
{
	int i;
	int numChildren=node1->numChildren;	
	C_ASTNode **tempAST=new C_ASTNode*[numChildren+1];

	for(i=0;i<numChildren;i++) 
		tempAST[i]=node1->children[i];
	tempAST[numChildren]=node2;

	if(node1->numChildren!=0) 
		delete [] node1->children;
	node1->children=tempAST;
	node1->numChildren++;
	
	return node1;
}

void C_DeleteAST(C_ASTNode *AST)
{
	int i;
	for(i=0;i<AST->numChildren;i++) C_DeleteAST(AST->children[i]);

	if(AST->numChildren>0) delete [] AST->children;
	delete AST;
}

void C_BuildASTNode(C_Token &newToken, C_Token *input, int number, int &error)
{
	C_ASTNode *tempNode;

#include "C_Actions.inc"

	newToken.line=input[0].line;
	newToken.astnode->line=newToken.line;
}
