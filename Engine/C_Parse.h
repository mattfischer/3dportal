#ifndef C_PARSE_H
#define C_PARSE_H

#include <string>

#include "C_ASTNode.h"

using std::string;

namespace Cog
{
    struct Tokenizer;

    ASTNode *ParseString( Tokenizer &t, const string& filename );
}

#endif
