#include "C_Stack.h"
#include "C_AST.h"
#include "M_Vector.h"

#include <assert.h>

C_Stack::C_Stack()
{
	stack=new char[STACKSIZE];
	pointer=STACKSIZE;
}

C_Stack::~C_Stack()
{
	delete[] stack;
}

void C_Stack::PushFloat(float a)
{
	Push(&a, sizeof(a));
}

void C_Stack::PushInt(int a)
{
	Push(&a, sizeof(a));
}

void C_Stack::PushVector(M_Vector a)
{
	Push(&a, sizeof(a));
}

void C_Stack::Push(void *data, int size)
{
	pointer-=size;
	memcpy(stack+pointer, data, size);
}

float C_Stack::PopFloat()
{
	float retVal;

	Pop((void*)&retVal, sizeof(retVal));
	return retVal;
}

int C_Stack::PopInt()
{
	int retVal;
	Pop(&retVal, sizeof(retVal));
	return retVal;
}

M_Vector C_Stack::PopVector()
{
	M_Vector retVal;

	Pop(&retVal, sizeof(retVal));
	return retVal;
}

void C_Stack::Pop(void *data, int size)
{
	assert(pointer+size<=STACKSIZE);

	memcpy(data, stack+pointer, size);
	pointer+=size;
}

float C_Stack::PopFlex(C_SymbolType destType)
{
	if(destType==C_TYPE_FLOAT) return PopFloat();
	else					   return PopInt();
}