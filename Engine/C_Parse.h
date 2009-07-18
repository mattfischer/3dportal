#ifndef C_PARSE_H
#define C_PARSE_H

#include <string>

using std::string;

namespace Cog
{
    struct ASTNode;
    struct Tokenizer;

    ASTNode *ParseString( Tokenizer &t, const string& filename );
}

#endif
