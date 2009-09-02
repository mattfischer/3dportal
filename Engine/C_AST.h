#ifndef NODE_H
#define NODE_H

#include "C_ASTNode.h"

namespace Cog
{   
    struct Token;

    void BuildASTNode(Token &newToken, Token *input, int num, int &error);
    void DeleteAST(ASTNode *AST);
}

#endif