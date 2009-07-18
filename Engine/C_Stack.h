#ifndef C_STACK_H
#define C_STACK_H

#include "C_AST.h"

#define STACKSIZE 100

class C_Stack
{
public:
	C_Stack();
	~C_Stack();

	void PushFloat( float a );
	void PushInt( int a );
	void PushVector( Math::Vector a );
	void Push( void *data, int size );

	float PopFloat();
	int PopInt();
	Math::Vector PopVector();
	float PopFlex( C_SymbolType destType );
	void Pop( void *data, int size );

protected:
	char *stack;
	int pointer;
};

#endif