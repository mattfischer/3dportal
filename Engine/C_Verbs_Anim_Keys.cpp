#include "C_Verb.h"
#include "R_Model.h"
#include "JK_Level.h"

extern R_Model* povModel;
extern JK_Key_Instance povKeyInstance;

C_Type_Void jkSetPovModel( C_Type_Thing thing_num, C_Type_Model model_num )
{
	povModel = currentLevel.models[model_num];
}

C_Type_Int jkPlayPovKey( C_Type_Thing thing_num, C_Type_Keyframe key, C_Type_Int param2, C_Type_Int flags )
{
    povKeyInstance = JK_Key_Instance( currentLevel.keyframes[key], 0, flags );

	return key;
}

C_Type_Int PlayKey( C_Type_Thing thing_num, C_Type_Keyframe key, C_Type_Int param2, C_Type_Int flags )
{
    return 0;
}

C_Type_Int GetKeyLen( C_Type_Keyframe key )
{
	return 0;
}