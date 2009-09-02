#ifndef C_TYPE_H
#define C_TYPE_H

#include "M_Vector.h"

enum _SymbolType { TYPE_AI, TYPE_COG, TYPE_FLEX, TYPE_THING=3, TYPE_FLOAT, 
				  TYPE_SECTOR=5, TYPE_SURFACE=6, TYPE_INT, TYPE_KEYFRAME, 
				  TYPE_MATERIAL, TYPE_MESSAGE, TYPE_MODEL, TYPE_SOUND, 
				  TYPE_TEMPLATE, TYPE_VECTOR};
typedef enum _SymbolType SymbolType;

namespace Cog
{
    typedef int Type_AI;
    typedef int Type_Cog;
    typedef float Type_Flex;
    typedef int Type_Thing;
    typedef float Type_Float;
    typedef int Type_Sector;
    typedef int Type_Surface;
    typedef int Type_Int;
    typedef int Type_Keyframe;
    typedef int Type_Material;
    typedef int Type_Message;
    typedef int Type_Model;
    typedef int Type_Sound;
    typedef int Type_Template;
    typedef Math::Vector Type_Vector;
    typedef void Type_Void;

    struct Type_Variant
    {
	    SymbolType type;
	    int v_int;
	    float v_float;
        Math::Vector v_vector;
    };
}

#endif