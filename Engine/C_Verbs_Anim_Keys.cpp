#include "C_Verb.h"
#include "R_Model.h"
#include "JK_Level.h"

extern R_Model* povModel;
extern Jk::Key::Track povKeyTrack;

namespace Cog
{
    Type_Void jkSetPovModel( Type_Thing thing_num, Type_Model model_num )
    {
	    povModel = currentLevel.models[model_num];
    }

    Type_Int jkPlayPovKey( Type_Thing thing_num, Type_Keyframe key, Type_Int param2, Type_Int flags )
    {
        povKeyTrack = Jk::Key::Track( currentLevel.keyframes[key], 0, flags );

	    return key;
    }

    Type_Int PlayKey( Type_Thing thing_num, Type_Keyframe key, Type_Int param2, Type_Int flags )
    {
        return 0;
    }

    Type_Int GetKeyLen( Type_Keyframe key )
    {
	    return 0;
    }
}