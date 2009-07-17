#ifndef C_VERB_H
#define C_VERB_H

#include "C_Type.h"
#include "C_Script.h"

/* -------- C_Verbs_Inventory.cpp -------- */
C_Type_Float GetInv( C_Type_Thing thing_num, C_Type_Int bin );
C_Type_Float GetInvMax( C_Type_Thing thing_num, C_Type_Int bin );
C_Type_Float GetInvMin( C_Type_Thing thing_num, C_Type_Int bin );
C_Type_Float ChangeInv( C_Type_Thing thing_num, C_Type_Int bin, C_Type_Float delta );

/* -------- C_Verbs_Anim_Key.cpp -------- */
C_Type_Void jkSetPovModel( C_Type_Thing thing_num, C_Type_Model model_num );
C_Type_Int jkPlayPovKey( C_Type_Thing thing_num, C_Type_Keyframe key, C_Type_Int param2, C_Type_Int flags );
C_Type_Int PlayKey( C_Type_Thing thing_num, C_Type_Keyframe key, C_Type_Int param2, C_Type_Int flags );
C_Type_Int GetKeyLen( C_Type_Keyframe key );

/* -------- C_Verbs_Output.cpp -------- */
C_Type_Void jkPrintUniString( C_Type_Int destination, C_Type_Int string );

/* -------- C_Verbs_Sector.cpp -------- */
C_Type_Void SetSectorAdjoins( C_Type_Sector sector_num, C_Type_Int status );
C_Type_Void SectorAdjoins( C_Type_Sector sector_num, C_Type_Int status );
C_Type_Void SetSectorLight( C_Type_Sector sector_num, C_Type_Flex light, C_Type_Flex delay );
C_Type_Void SectorLight( C_Type_Sector sector_num, C_Type_Flex light, C_Type_Flex delay );

/* -------- C_VerbSound::Buffer.cpp -------- */
C_Type_Int PlaySoundLocal( C_Type_Sound sound_num, C_Type_Flex volume, C_Type_Flex pan, C_Type_Int flags );
C_Type_Int PlaySoundPos( C_Type_Sound sound_num, C_Type_Vector position, C_Type_Flex volume, C_Type_Flex minDistance, C_Type_Flex maxDistance, C_Type_Int flags );
C_Type_Int PlaySoundThing( C_Type_Sound sound_num, C_Type_Thing thing_num, C_Type_Flex volume, C_Type_Flex minDistance, C_Type_Flex maxDistance, C_Type_Int flags ); 
C_Type_Int GetSoundLen( C_Type_Sound sound_num );

/* -------- C_Verbs_Special_Effects.cpp -------- */
C_Type_Void AddDynamicTint( C_Type_Thing thing_num, C_Type_Float red, C_Type_Float gree, C_Type_Float blue );

/* -------- C_Verbs_Surface.cpp -------- */
C_Type_Int SetWallCel( C_Type_Surface surface_num, C_Type_Int cel );
C_Type_Int GetWallCel( C_Type_Surface surface_num );
C_Type_Vector GetSurfaceCenter( C_Type_Surface surface_num );
C_Type_Vector SurfaceCenter( C_Type_Surface surface_num );
C_Type_Void SurfaceAnim( C_Type_Surface surface_num, C_Type_Flex fps, C_Type_Int flags );
C_Type_Void StopSurfaceAnim( C_Type_Surface surface_num );
C_Type_Void SlideWall( C_Type_Surface surface_num, C_Type_Vector direction, C_Type_Flex speed );

/* -------- C_Verbs_System.cpp -------- */
C_Type_Int jkGetSaberCam();
C_Type_Int GetCurrentCamera();
C_Type_Int GetPrimaryFocus();
C_Type_Void sleep( C_Type_Float delay );
C_Type_Float Rand();
C_Type_Void TakeItem( C_Type_Thing thing_num, C_Type_Thing player_num );
C_Type_Int GetAutoSwitch();
C_Type_Variant GetSenderRef( C_Context& c );
C_Type_Int GetSourceRef( C_Context& c );
C_Type_Int GetSenderId( C_Context& c );
C_Type_Int GetDifficulty();
C_Type_Float GetLevelTime();
C_Type_Void SetTimer( C_Type_Flex t, C_Script* script );
C_Type_Vector VectorSet( C_Type_Flex x, C_Type_Flex y, C_Type_Flex z );
C_Type_Vector VectorScale( C_Type_Vector v, C_Type_Flex scale );
C_Type_Vector RandVec();

/* -------- C_Verbs_Thing.cpp -------- */
C_Type_Int GetCurFrame( C_Type_Thing thing_num );
C_Type_Void MoveToFrame( C_Type_Thing thing_num, C_Type_Int frame, C_Type_Flex speed );
C_Type_Sector GetThingSector( C_Type_Thing thing_num );
C_Type_Thing GetLocalPlayerThing();
C_Type_Thing jkGetLocalPlayer();
C_Type_Int IsThingMoving( C_Type_Thing thing_num );
C_Type_Int IsMoving( C_Type_Thing thing_num );
C_Type_Void JumpToFrame( C_Type_Thing thing_num, C_Type_Int frame, C_Type_Sector sector_num );
C_Type_Float GetThingHealth( C_Type_Thing thing_num );
C_Type_Float GetHealth( C_Type_Thing thing_num );
C_Type_Void DestroyThing( C_Type_Thing thing_num );
C_Type_Thing CreateThing( C_Type_Template template_num, C_Type_Thing position_num );
C_Type_Void CaptureThing( C_Type_Thing thing_num, C_Script* script );
C_Type_Vector GetThingLVec( C_Type_Thing thing_num );

/* -------- C_Verbs_Weapon.cpp -------- */
C_Type_Int GetAutoReload();
C_Type_Int GetAutoPickup();
C_Type_Int GetWeaponPriority( C_Type_Int thing_num, C_Type_Int weapon, C_Type_Int select_mode );
C_Type_Int GetCurWeapon( C_Type_Thing thing_num, C_Type_Int param );
C_Type_Void SelectWeapon( C_Type_Thing thing_num, C_Type_Int bin );
C_Type_Int AutoselectWeapon( C_Type_Thing thing_num, C_Type_Int bin );
C_Type_Thing FireProjectile( C_Type_Thing thing_num, C_Type_Template template_num, C_Type_Sound sound_num, C_Type_Int mode, C_Type_Vector offset, C_Type_Vector error, C_Type_Flex unk, C_Type_Int flags, C_Type_Flex fov, C_Type_Flex maxDistance );
C_Type_Void SetCurWeapon( C_Type_Thing thing_num, C_Type_Int bin );

#endif