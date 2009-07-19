#ifndef R_THING_H
#define R_THING_H

#include "M_Vector.h"
#include "W_Poly.h"

#include "R_Sprite.h"

#include "JK_Template.h"
#include "JK_Key.h"
#include "JK_AnimClass.h"

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;

namespace Render
{
    class Model;
    struct Node;
    struct Frustum;
}
namespace Sound
{
    class Class;
    class Track;
}
namespace Cog
{
    class Script;
}

void JK_Level_Load( const string& name );

namespace World
{
    class Sector;
    class Surface;

    struct Frame {
	    Math::Vector position;
	    Math::Vector rotation;
    };

    class Thing
    {
	    enum MoveType {MOVE_NONE, MOVE_PATH, MOVE_PHYSICS};
    	
        friend void ::JK_Level_Load( const string& name );
    public:
	    enum Type { ACTOR, WEAPON, ITEM, EXPLOSION, COG, GHOST, CORPSE, PLAYER, PARTICLE};

	    // Thing.cpp
        Thing( Jk::Template *t, Math::Vector p, Math::Vector r, Sector *s );
	    Thing( Thing &c );

        static int Create( Jk::Template *t, Math::Vector p, Math::Vector r, Sector *s );
        static int CreateFromThing( Thing *thing, Jk::Template *newTemplate );

	    virtual ~Thing();
	    Thing &operator=( Thing &c );

	    Math::Vector GetPosition();
	    void SetPosition( Math::Vector p );
	    Math::Vector GetRotation();
	    void SetRotation( Math::Vector r );
	    Sector *GetSector();
	    void SetSector( Sector *newSector );
	    Math::Vector GetCompositeRotation();

	    Math::Vector GetAcceleration();
	    Math::Vector GetVelocity();
	    int GetFlagValue();
	    void Destroy();

	    int GetType();
	    int GetTypeFlags();
	    int GetThingFlags();
	    int GetPhysicsFlags();
	    int GetCollide();
        Jk::Template *GetTemplate();
        void SetTemplate( Jk::Template* t );

	    Render::Model *GetModel();
	    float GetMoveSize();
	    float GetSize();
	    float GetMass();
	    float GetMaxThrust();

	    void SetAttached( Surface *s );
	    void SetAttached( Poly *p, Thing *t );
	    bool IsAttached();
	    bool WasAttached();
	    Surface *GetAttachSurface();

	    Math::Vector GetEyePosition();
	    void SetCrouched( bool c );
	    void Jump();
	    Math::Vector GetStandVector();

	    void Update( float time );

	    void AddCogLink( Cog::Script *cogScript );
	    void SendCogMessages( const string& message, int source, bool synchronous = false );

        void playKey( Jk::Key *key, int flags );

        int GetNum();

        void addSound( Sound::Track *track );
        void removeSound( Sound::Track *track );

	    // P_Thing.cpp
	    static void UpdateThings( float time );

	    void ApplyThrust( Math::Vector thrust );
    	
	    void AddVelocity( Math::Vector vel );
	    void SetVelocity( Math::Vector vel );
    	
	    void Nudge( Math::Vector n );

	    int GetCurrentFrame();
	    bool IsPathMove();
	    bool IsPhysicsMove();
	    Math::Vector GetPathMoveDelta();
	    void MoveToFrame( int frame, float speed );
	    void JumpToFrame( int frame, Sector *s );
	    bool IsMoving();
    	
	    void Activate();
    	
	    void ArrestMotion( Math::Vector normal );

	    bool PerformActivate( Math::Vector position, Math::Vector point );
	    void ThingCollisions( Thing* thing );
	    bool ThingFloorCollisions( Thing* thing );
	    void ThingSurfaceCollisions( Thing* thing );

	    // JK_Thing.cpp
	    void ProcessTemplate();
	    void Explode();

	    // R_Thing.cpp
	    void Draw( Render::Frustum frustum, float light, Math::Vector tint );

    protected:

	    //int num;
	    Math::Vector position;
	    Math::Vector rotation;
	    Math::Vector velocity;
	    Math::Vector acceleration;
	    Sector *sector;

	    Math::Vector nudge;
    	
        Jk::Template *thingTemplate;
	    Render::Model *model;
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
	    Math::Vector eyeOffset;
	    int typeFlags;
	    int thingFlags;
	    int physicsFlags;
	    Math::Vector rotVelocity;
	    MoveType move;
	    int collide;
	    float jumpVelocity;

	    Sound::Class *soundClass;
	    Math::Vector orient;

	    Math::Vector insertOffset;

	    int attachFlags;
	    int oldAttachFlags;
	    Surface *attachSurface;
	    Poly attachPoly;
	    Thing *attachThing;

	    int flagValue;
    	
	    bool jumping;
	    bool thrusting;
	    bool crouched;
    	
	    bool pathMoving;
	    int currentFrame;
	    int destFrame;
	    Math::Vector pathVector;
	    float pathT;
	    float pathTSpeed;
	    Math::Vector pathMoveDelta;
	    vector<Frame> frames;
	    vector<Cog::Script*> cogLinks;

	    Sound::Track *moveSound;
	    LONG walkTimer;
	    int walkSide;

	    Type type;

        Jk::Key::Track keyTrack;
    	
        Jk::AnimClass *animClass;

        Jk::Template *explodeTemplate;

	    Render::Sprite *sprite;
	    float spriteTime;

	    LONG killTime;

        std::vector<Sound::Track*> sounds;

        int num;
        bool pendingDestroy;

	    // P_Thing.cpp
	    void UpdatePath(float time);
	    void UpdateForces(float time);
	    void UpdateFinalize(float time);

	    // Thing.cpp
	    void DoFoley();
	    int GetSurfaceType();
        void RealDestroy();
    };
}
#endif