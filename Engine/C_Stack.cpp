#include "C_Stack.h"
#include "C_AST.h"
#include "M_Vector.h"

#include <assert.h>

namespace Cog
{
    Stack::Stack()
    {
	    stack=new char[STACKSIZE];
	    pointer=STACKSIZE;
    }

    Stack::~Stack()
    {
	    delete[] stack;
    }

    void Stack::PushFloat(float a)
    {
	    Push(&a, sizeof(a));
    }

    void Stack::PushInt(int a)
    {
	    Push(&a, sizeof(a));
    }

    void Stack::PushVector(Math::Vector a)
    {
	    Push(&a, sizeof(a));
    }

    void Stack::Push(void *data, int size)
    {
	    pointer-=size;
	    memcpy(stack+pointer, data, size);
    }

    float Stack::PopFloat()
    {
	    float retVal;

	    Pop((void*)&retVal, sizeof(retVal));
	    return retVal;
    }

    int Stack::PopInt()
    {
	    int retVal;
	    Pop(&retVal, sizeof(retVal));
	    return retVal;
    }

    Math::Vector Stack::PopVector()
    {
	    Math::Vector retVal;

	    Pop(&retVal, sizeof(retVal));
	    return retVal;
    }

    void Stack::Pop(void *data, int size)
    {
	    assert(pointer+size<=STACKSIZE);

	    memcpy(data, stack+pointer, size);
	    pointer+=size;
    }

    float Stack::PopFlex(SymbolType destType)
    {
	    if(destType==TYPE_FLOAT) return PopFloat();
	    else					   return PopInt();
    }
}