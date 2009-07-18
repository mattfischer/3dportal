#include "C_Verb.h"

#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "W_Sector.h"
#include "JK_Level.h"
#include "S_Manager.h"
#include <math.h>
#include "U_Matrix.h"

int curWeapon = -1;

C_Type_Int GetAutoReload() // ****
{	
	return 0;
}

C_Type_Int GetAutoPickup() // ****
{	
	return 0;
}

C_Type_Int GetWeaponPriority( C_Type_Int thing_num, C_Type_Int weapon, C_Type_Int select_mode ) // ****
{
	return 0;
}

C_Type_Int GetCurWeapon( C_Type_Thing thing_num, C_Type_Int param ) // ?????
{
	return curWeapon;
}

C_Type_Void SelectWeapon( C_Type_Thing thing_num, C_Type_Int bin )
{
	
}
	
C_Type_Int AutoselectWeapon( C_Type_Thing thing_num, C_Type_Int bin )
{
	return 0;
}

C_Type_Thing FireProjectile( C_Type_Thing thing_num, C_Type_Template template_num, C_Type_Sound sound_num, C_Type_Int mode, C_Type_Vector offset, C_Type_Vector error, C_Type_Flex unk, C_Type_Int flags, C_Type_Flex fov, C_Type_Flex maxDistance )
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

C_Type_Void SetCurWeapon( C_Type_Thing thing_num, C_Type_Int bin )
{
	curWeapon = bin;
}
