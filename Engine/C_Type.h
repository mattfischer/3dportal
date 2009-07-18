#ifndef C_TYPE_H
#define C_TYPE_H

#include "M_Vector.h"

enum C_SymbolType { C_TYPE_AI, C_TYPE_COG, C_TYPE_FLEX, C_TYPE_THING=3, C_TYPE_FLOAT, 
					C_TYPE_SECTOR=5, C_TYPE_SURFACE=6, C_TYPE_INT, C_TYPE_KEYFRAME, 
					C_TYPE_MATERIAL, C_TYPE_MESSAGE, C_TYPE_MODEL, C_TYPE_SOUND, 
					C_TYPE_TEMPLATE, C_TYPE_VECTOR};

typedef int C_Type_AI;
typedef int C_Type_Cog;
typedef float C_Type_Flex;
typedef int C_Type_Thing;
typedef float C_Type_Float;
typedef int C_Type_Sector;
typedef int C_Type_Surface;
typedef int C_Type_Int;
typedef int C_Type_Keyframe;
typedef int C_Type_Material;
typedef int C_Type_Message;
typedef int C_Type_Model;
typedef int C_Type_Sound;
typedef int C_Type_Template;
typedef Math::Vector C_Type_Vector;
typedef void C_Type_Void;

struct C_Type_Variant
{
	C_SymbolType type;
	int v_int;
	float v_float;
    Math::Vector v_vector;
};

#endif