#include "C_Verb.h"
#include "R_Model.h"
#include "JK_Level.h"

extern R_Model* povModel;

C_Type_Void jkSetPovModel( C_Type_Thing thing_num, C_Type_Model model_num )
{
	povModel = currentLevel.models[model_num];
}

C_Type_Int jkPlayPovKey( C_Type_Int param0, C_Type_Int param1, C_Type_Int param2, C_Type_Int param3 )
{
	return 0;
}

C_Type_Int GetKeyLen( C_Type_Keyframe key )
{
	return 0;
}