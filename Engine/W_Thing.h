#ifndef R_THING_H
#define R_THING_H

#include "Global.h"

#include "M_Vector.h"
#include "W_Poly.h"

#include "R_Sprite.h"

#include "JK_Template.h"
#include "JK_Key.h"
#include "JK_AnimClass.h"

#include "U_Collection.h"

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;

class W_Sector;
class R_Model;
struct R_Node;
struct R_Frustum;
class W_Surface;
struct S_SoundClass;
class S_Sound;
class C_Script;

struct W_Frame {
	M_Vector position;
	M_Vector rotation;
};


class W_Thing : public U_RefCountedItem, public U_NumerableItem
{
	enum MoveType {MOVE_NONE, MOVE_PATH, MOVE_PHYSICS};
	
	friend void JK_Level_Load( const string& name );
public:
	enum Type { ACTOR, WEAPON, ITEM, EXPLOSION, COG, GHOST, CORPSE, PLAYER, PARTICLE};

	// W_Thing.cpp
	W_Thing( JK_Template *t, M_Vector p, M_Vector r, W_Sector *s );
	W_Thing( W_Thing &c );

	static int Create( JK_Template *t, M_Vector p, M_Vector r, W_Sector *s );
	static int CreateFromThing( W_Thing *thing, JK_Template *newTemplate );

	virtual ~W_Thing();
	W_Thing &operator=( W_Thing &c );

	M_Vector GetPosition();
	void SetPosition( M_Vector p );
	M_Vector GetRotation();
	void SetRotation( M_Vector r );
	W_Sector *GetSector();
	void SetSector( W_Sector *newSector );
	M_Vector GetCompositeRotation();

	M_Vector GetAcceleration();
	M_Vector GetVelocity();
	int GetFlagValue();
	void Destroy();

	int GetType();
	int GetTypeFlags();
	int GetThingFlags();
	int GetPhysicsFlags();
	int GetCollide();
	JK_Template *GetTemplate();
	void SetTemplate( JK_Template* t );

	R_Model *GetModel();
	float GetMoveSize();
	float GetSize();
	float GetMass();
	float GetMaxThrust();

	void SetAttached( W_Surface *s );
	void SetAttached( W_Poly *p, W_Thing *t );
	bool IsAttached();
	bool WasAttached();
	W_Surface *GetAttachSurface();

	M_Vector GetEyePosition();
	void SetCrouched( bool c );
	void Jump();
	M_Vector GetStandVector();

	void Update( float time );

	void AddCogLink( C_Script *cogScript );
	void SendCogMessages( const string& message, int source, bool synchronous = false );

	void playKey( JK_Key *key, int flags );

	// P_Thing.cpp
	static void UpdateThings( float time );

	void ApplyThrust( M_Vector thrust );
	
	void AddVelocity( M_Vector vel );
	void SetVelocity( M_Vector vel );
	
	void Nudge( M_Vector n );

	int GetCurrentFrame();
	bool IsPathMove();
	bool IsPhysicsMove();
	M_Vector GetPathMoveDelta();
	void MoveToFrame( int frame, float speed );
	void JumpToFrame( int frame, W_Sector *s );
	bool IsMoving();
	
	void Activate();
	
	void ArrestMotion( M_Vector normal );

	bool PerformActivate( M_Vector position, M_Vector point );
	void ThingCollisions( W_Thing* thing );
	bool ThingFloorCollisions( W_Thing* thing );
	void ThingSurfaceCollisions( W_Thing* thing );

	// JK_Thing.cpp
	void ProcessTemplate();
	void Explode();

	// R_Thing.cpp
	void Draw( R_Frustum frustum, float light, M_Vector tint );

protected:

	//int num;
	M_Vector position;
	M_Vector rotation;
	M_Vector velocity;
	M_Vector acceleration;
	W_Sector *sector;

	M_Vector nudge;
	
	JK_Template *thingTemplate;
	R_Model *model;
	float maxVelocity;
	float maxThrust;
	float maxRotVelocity;
	float maxRotThrust;
	float mass;
	float surfaceDrag;
	float staticDrag;
	float airDrag;
	float size;
	float moveSize;
	M_Vector eyeOffset;
	int typeFlags;
	int thingFlags;
	int physicsFlags;
	M_Vector rotVelocity;
	MoveType move;
	int collide;
	float jumpVelocity;

	S_SoundClass *soundClass;
	M_Vector orient;

	M_Vector insertOffset;

	int attachFlags;
	int oldAttachFlags;
	W_Surface *attachSurface;
	W_Poly attachPoly;
	W_Thing *attachThing;

	int flagValue;
	
	bool jumping;
	bool thrusting;
	bool crouched;
	
	bool pathMoving;
	int currentFrame;
	int destFrame;
	M_Vector pathVector;
	float pathT;
	float pathTSpeed;
	M_Vector pathMoveDelta;
	vector<W_Frame> frames;
	vector<C_Script*> cogLinks;

	S_Sound *moveSound;
	LONG walkTimer;
	int walkSide;

	Type type;

	JK_Key_Instance keyInstance;
	
	JK_AnimClass *animClass;

	JK_Template *explodeTemplate;

	R_Sprite *sprite;
	int spriteFrame;

	LONG killTime;

	// P_Thing.cpp
	void UpdatePath(float time);
	void UpdateForces(float time);
	void UpdateFinalize(float time);

	// W_Thing.cpp
	void DoFoley();
	int GetSurfaceType();
};

#endif