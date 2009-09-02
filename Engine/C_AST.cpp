#include <stdarg.h>

#include "C_Tokenizer.h"
#include "C_AST.h"

namespace Cog
{
    ASTNode *NewNode(C_AST_NodeType type, int numArgs, ...)
    {
	    va_list list;
	    int i;

	    va_start(list, numArgs);
	    ASTNode *astnode;
	    astnode=new ASTNode;
	    astnode->nodeType=type;
	    astnode->children=new ASTNode*[numArgs];
	    astnode->numChildren=numArgs;
	    for(i=0;i<numArgs;i++)
		    astnode->children[i]=va_arg(list, ASTNode*);
    	
	    return astnode;
    }

    ASTNode *AddChildren(ASTNode *node1, ASTNode *node2)
    {
	    int i;
	    ASTNode **tempAST=new ASTNode*[node1->numChildren+node2->numChildren];

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

    ASTNode *AddNode(ASTNode *node1, ASTNode *node2)
    {
	    int i;
	    int numChildren=node1->numChildren;	
	    ASTNode **tempAST=new ASTNode*[numChildren+1];

	    for(i=0;i<numChildren;i++) 
		    tempAST[i]=node1->children[i];
	    tempAST[numChildren]=node2;

	    if(node1->numChildren!=0) 
		    delete [] node1->children;
	    node1->children=tempAST;
	    node1->numChildren++;
    	
	    return node1;
    }

    void DeleteAST(ASTNode *AST)
    {
	    int i;
	    for(i=0;i<AST->numChildren;i++) DeleteAST(AST->children[i]);

	    if(AST->numChildren>0) delete [] AST->children;
	    delete AST;
    }

    void BuildASTNode(Token &newToken, Token *input, int number, int &error)
    {
	    ASTNode *tempNode;

    #include "C_Actions.inc"

	    newToken.line=input[0].line;
	    newToken.astnode->line=newToken.line;
    }
}