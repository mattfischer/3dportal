#include "C_Verb.h"

#include <boost/shared_ptr.hpp>
#include "W_Thing.h"
#include "W_Sector.h"
#include "JK_Level.h"
#include "S_Manager.h"
#include <math.h>

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
	M_Vector rotation;
	float xcos, xsin;
	float ycos, ysin;
	int retval;
	M_Vector fireOffset;

	thing = currentLevel.things[thing_num];

	if( thing )
		rotation = thing->GetRotation();

	xcos = cos( rotation.y * 3.14 / 180 );
	xsin = sin( rotation.y * 3.14 / 180 );

	M_Matrix m( xcos, -xsin, 0, 0,
			    xsin,  xcos, 0, 0,
			    0,     0,    1, 0,
			    0,     0,    0, 1
			  );

	ycos = cos( rotation.x * 3.14 / 180 );
	ysin = sin( rotation.x * 3.14 / 180 );

	M_Matrix m2( 1, 0,    0,    0,
			     0, ycos,-ysin, 0,
			     0, ysin, ycos, 0,
			     0, 0,    0,    1
			);
	fireOffset = m * m2 * offset;

	shared_ptr<W_Thing> new_thing( new W_Thing( currentLevel.templates[template_num], thing->GetPosition() + fireOffset, M_Vector(0,0,0), thing->GetSector() ) );

	retval = currentLevel.things.size();
	new_thing->SetNum( retval );
	currentLevel.things.push_back( new_thing );
	
	new_thing->ProcessTemplate();
	new_thing->SetRotation( thing->GetRotation() );
	new_thing->GetSector()->AddThing( new_thing.get() );

	if( sound_num != -1 )
		S_PlayLocal( currentLevel.sounds[sound_num], 1.0, 0 );

	return retval;
}

C_Type_Void SetCurWeapon( C_Type_Thing thing_num, C_Type_Int bin )
{
	curWeapon = bin;
}
