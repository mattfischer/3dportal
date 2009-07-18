#include "C_Verb.h"

#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "W_Sector.h"
#include "JK_Level.h"
#include "S_Manager.h"
#include <math.h>
#include "U_Matrix.h"

int curWeapon = -1;

namespace Cog
{
    Type_Int GetAutoReload() // ****
    {	
	    return 0;
    }

    Type_Int GetAutoPickup() // ****
    {	
	    return 0;
    }

    Type_Int GetWeaponPriority( Type_Int thing_num, Type_Int weapon, Type_Int select_mode ) // ****
    {
	    return 0;
    }

    Type_Int GetCurWeapon( Type_Thing thing_num, Type_Int param ) // ?????
    {
	    return curWeapon;
    }

    Type_Void SelectWeapon( Type_Thing thing_num, Type_Int bin )
    {
    	
    }
    	
    Type_Int AutoselectWeapon( Type_Thing thing_num, Type_Int bin )
    {
	    return 0;
    }

    Type_Thing FireProjectile( Type_Thing thing_num, Type_Template template_num, Type_Sound sound_num, Type_Int mode, Type_Vector offset, Type_Vector error, Type_Flex unk, Type_Int flags, Type_Flex fov, Type_Flex maxDistance )
    {
	    shared_ptr<W_Thing> thing;
	    Math::Vector rotation;
	    float xcos, xsin;
	    float ycos, ysin;
	    int retval;
	    Math::Vector fireOffset;

	    thing = currentLevel.things[thing_num];

	    if( thing )
		    rotation = thing->GetRotation();

        fireOffset = Util::Matrix::RotateZ(rotation.y) * Util::Matrix::RotateX(rotation.x) * offset;
	    int thingNum = W_Thing::Create( currentLevel.templates[template_num], thing->GetPosition() + fireOffset, Math::Vector(0, 0, 0), thing->GetSector());

	    currentLevel.things[thingNum]->SetRotation( thing->GetRotation() );
	    if( sound_num != -1 )
            Sound::PlayLocal( currentLevel.sounds[sound_num], 1.0, 0 );

	    return thingNum;
    }

    Type_Void SetCurWeapon( Type_Thing thing_num, Type_Int bin )
    {
	    curWeapon = bin;
    }
}