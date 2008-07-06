#ifndef W_SECTOR_H
#define W_SECTOR_H

#include "M_Vector.h"

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;
using std::vector;

class W_Thing;
class W_Surface;
struct R_Frustum;
class M_Vector;
class C_Script;
class S_Sound;

class W_Sector {
	
	friend void JK_Level_Load( const string& name );

public:
	// W_Sector.cpp
	W_Sector();
	int NumSurfaces();
	W_Surface *operator[]( int n );

	void RemoveThing( W_Thing* thing );
	void AddThing( W_Thing* thing );

	void AddCogLink( C_Script *cogScript );
	void SendCogMessages( const string& message, int source, bool synchronous = false );

	int GetNum();
	void RenderAdjoins( bool r );
	void SetLight( float l );

	void PlaySectorSound();

	// R_Sector.cpp
	void Draw( R_Frustum frustum, W_Surface *ignore );

	// P_Sector.cpp
	bool UpdateThingSector( W_Thing* thing, M_Vector oldPosition );
	void SurfaceCollisions( W_Thing* thing, W_Surface *ignore );
	bool FloorCollisions( W_Thing* thing, W_Surface *ignore );
	void ThingCollisions( W_Thing* thing, W_Surface *ignore );
	bool PerformActivate( M_Vector position, M_Vector point, W_Surface *ignore );

	bool ThingFloorCollisions( W_Thing* thing, W_Surface *ignore );
	void ThingSurfaceCollisions( W_Thing* thing, W_Surface *ignore );

protected:
	int listname;
	int num;
	float ambientLight;
	float extraLight;
	
	M_Vector tint;
	
	int numSurfaces;
	W_Surface **surfaces;
	vector<int> things;
	vector<C_Script*> cogLinks;

	S_Sound *sound;
	float soundVolume;

	bool renderAdjoins;

	bool drawing;
	bool recurseGuard;

	CRITICAL_SECTION critSec;
};

#endif