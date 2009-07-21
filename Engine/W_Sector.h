#ifndef W_SECTOR_H
#define W_SECTOR_H

#include "M_Vector.h"

#include <string>
#include <vector>

#include "JK_Level.h"

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using std::string;
using std::vector;

namespace Render
{
    struct Frustum;
}
namespace Cog
{
    class Script;
}
namespace Sound
{
    class Buffer;
}

namespace World
{
    class Thing;
    class Surface;
    class Sector {
        friend void Jk::Level::Load( const string& name );

    public:
	    // Sector.cpp
	    Sector();
	    int NumSurfaces();
	    Surface *operator[]( int n );

	    void RemoveThing( Thing* thing );
	    void AddThing( Thing* thing );

        void AddCogLink( Cog::Script *cogScript );
	    void SendCogMessages( const string& message, int source, bool synchronous = false );

	    int GetNum();
	    void RenderAdjoins( bool r );
	    void SetLight( float l );

	    void PlaySectorSound();

	    // R_Sector.cpp
	    void Draw( Render::Frustum frustum, Surface *ignore );

	    // P_Sector.cpp
	    bool UpdateThingSector( Thing* thing, Math::Vector oldPosition );
	    void SurfaceCollisions( Thing* thing, Surface *ignore );
	    bool FloorCollisions( Thing* thing, Surface *ignore );
	    void ThingCollisions( Thing* thing, Surface *ignore );
	    bool PerformActivate( Math::Vector position, Math::Vector point, Surface *ignore );

	    bool ThingFloorCollisions( Thing* thing, Surface *ignore );
	    void ThingSurfaceCollisions( Thing* thing, Surface *ignore );

    protected:
	    int listname;
	    int num;
	    float ambientLight;
	    float extraLight;
    	
	    Math::Vector tint;
    	
	    int numSurfaces;
	    Surface **surfaces;
	    vector<Thing*> things;
        vector<Cog::Script*> cogLinks;

	    Sound::Buffer *sound;
	    float soundVolume;

	    bool renderAdjoins;

	    bool drawing;
	    bool recurseGuard;
    };
}
#endif