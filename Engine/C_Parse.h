#ifndef C_PARSE_H
#define C_PARSE_H

#include <string>

using std::string;

struct C_ASTNode;
struct C_Tokenizer;

C_ASTNode *C_ParseString( C_Tokenizer &t, const string& filename );

#endif
