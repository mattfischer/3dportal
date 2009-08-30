#ifndef C_VERB_H
#define C_VERB_H

#include "C_Type.h"
#include "C_Script.h"

namespace Cog
{
    /* -------- C_Verbs_Inventory.cpp -------- */
    Type_Float GetInv( Type_Thing thing_num, Type_Int bin );
    Type_Float GetInvMax( Type_Thing thing_num, Type_Int bin );
    Type_Float GetInvMin( Type_Thing thing_num, Type_Int bin );
    Type_Float ChangeInv( Type_Thing thing_num, Type_Int bin, Type_Float delta );

    /* -------- C_Verbs_Anim_Key.cpp -------- */
    Type_Void jkSetPovModel( Type_Thing thing_num, Type_Model model_num );
    Type_Int jkPlayPovKey( Type_Thing thing_num, Type_Keyframe key, Type_Int param2, Type_Int flags );
    Type_Int PlayKey( Type_Thing thing_num, Type_Keyframe key, Type_Int param2, Type_Int flags );
    Type_Int GetKeyLen( Type_Keyframe key );

    /* -------- C_Verbs_Output.cpp -------- */
    Type_Void jkPrintUniString( Type_Int destination, Type_Int string );

    /* -------- C_Verbs_Sector.cpp -------- */
    Type_Void SetSectorAdjoins( Type_Sector sector_num, Type_Int status );
    Type_Void SectorAdjoins( Type_Sector sector_num, Type_Int status );
    Type_Void SetSectorLight( Type_Sector sector_num, Type_Flex light, Type_Flex delay );
    Type_Void SectorLight( Type_Sector sector_num, Type_Flex light, Type_Flex delay );

    /* -------- C_VerbSound::Buffer.cpp -------- */
    Type_Int PlaySoundLocal( Type_Sound sound_num, Type_Flex volume, Type_Flex pan, Type_Int flags );
    Type_Int PlaySoundPos( Type_Sound sound_num, Type_Vector position, Type_Flex volume, Type_Flex minDistance, Type_Flex maxDistance, Type_Int flags );
    Type_Int PlaySoundThing( Type_Sound sound_num, Type_Thing thing_num, Type_Flex volume, Type_Flex minDistance, Type_Flex maxDistance, Type_Int flags ); 
    Type_Int GetSoundLen( Type_Sound sound_num );

    /* -------- C_Verbs_Special_Effects.cpp -------- */
    Type_Void AddDynamicTint( Type_Thing thing_num, Type_Float red, Type_Float gree, Type_Float blue );

    /* -------- C_Verbs_Surface.cpp -------- */
    Type_Int SetWallCel( Type_Surface surface_num, Type_Int cel );
    Type_Int GetWallCel( Type_Surface surface_num );
    Type_Vector GetSurfaceCenter( Type_Surface surface_num );
    Type_Vector SurfaceCenter( Type_Surface surface_num );
    Type_Void SurfaceAnim( Type_Surface surface_num, Type_Flex fps, Type_Int flags );
    Type_Void StopSurfaceAnim( Type_Surface surface_num );
    Type_Void SlideWall( Type_Surface surface_num, Type_Vector direction, Type_Flex speed );

    /* -------- C_Verbs_System.cpp -------- */
    Type_Int jkGetSaberCam();
    Type_Int GetCurrentCamera();
    Type_Int GetPrimaryFocus();
    Type_Void sleep( Type_Float delay );
    Type_Float Rand();
    Type_Void TakeItem( Type_Thing thing_num, Type_Thing player_num );
    Type_Int GetAutoSwitch();
    Type_Variant GetSenderRef( Context& c );
    Type_Int GetSourceRef( Context& c );
    Type_Int GetSenderId( Context& c );
    Type_Int GetDifficulty();
    Type_Float GetLevelTime();
    Type_Void SetTimer( Type_Flex t, Script* script );
    Type_Vector VectorSet( Type_Flex x, Type_Flex y, Type_Flex z );
    Type_Vector VectorScale( Type_Vector v, Type_Flex scale );
    Type_Vector RandVec();

    /* -------- C_Verbs_Thing.cpp -------- */
    Type_Int GetCurFrame( Type_Thing thing_num );
    Type_Void MoveToFrame( Type_Thing thing_num, Type_Int frame, Type_Flex speed );
    Type_Sector GetThingSector( Type_Thing thing_num );
    Type_Thing GetLocalPlayerThing();
    Type_Thing jkGetLocalPlayer();
    Type_Int IsThingMoving( Type_Thing thing_num );
    Type_Int IsMoving( Type_Thing thing_num );
    Type_Void JumpToFrame( Type_Thing thing_num, Type_Int frame, Type_Sector sector_num );
    Type_Float GetThingHealth( Type_Thing thing_num );
    Type_Float GetHealth( Type_Thing thing_num );
    Type_Void DestroyThing( Type_Thing thing_num );
    Type_Thing CreateThing( Type_Template template_num, Type_Thing position_num );
    Type_Void CaptureThing( Type_Thing thing_num, Script* script );
    Type_Vector GetThingLVec( Type_Thing thing_num );
    Type_Cog GetThingClassCog( Type_Thing thing_num );

    /* -------- C_Verbs_Weapon.cpp -------- */
    Type_Int GetAutoReload();
    Type_Int GetAutoPickup();
    Type_Int GetWeaponPriority( Type_Int thing_num, Type_Int weapon, Type_Int select_mode );
    Type_Int GetCurWeapon( Type_Thing thing_num, Type_Int param );
    Type_Void SelectWeapon( Type_Thing thing_num, Type_Int bin );
    Type_Int AutoselectWeapon( Type_Thing thing_num, Type_Int bin );
    Type_Thing FireProjectile( Type_Thing thing_num, Type_Template template_num, Type_Sound sound_num, Type_Int mode, Type_Vector offset, Type_Vector error, Type_Flex unk, Type_Int flags, Type_Flex fov, Type_Flex maxDistance );
    Type_Void SetCurWeapon( Type_Thing thing_num, Type_Int bin );
}

#endif