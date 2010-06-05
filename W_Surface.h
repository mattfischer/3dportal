#ifndef W_SURFACE_H
#define W_SURFACE_H

#include "W_Poly.h"

#include "JK_Level.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;

namespace Cog
{
    class Script;
}

namespace World
{
    class Thing;
    class Sector;

    class Surface {
        friend void Jk::Level::Load( const string& name );

    public:
	    Surface();

	    bool Adjoined();
	    int GetFlags();
	    int GetGeo();
	    int GetFace();
	    int GetAdjoinFlags();

	    const Poly &GetPoly();
	    Sector *GetAdjoin();
	    Surface *GetMirror();

        void AddCogLink( Cog::Script *cogScript );
	    void SendCogMessages( const string& message, int source, bool synchronous=false);

	    void SetCel( int c );
	    int GetCel();

	    void Animate( float fps, int flags );
	    void StopAnimate();
	    void Slide( Math::Vector direction, float speed );

	    void Update();

	    Math::Vector GetMoveDelta();

	    // R_Surface.cpp
	    void Draw( Math::Vector tint, float light );

	    // P_Surface.cpp
	    bool PerformActivate( Math::Vector position, Math::Vector point );
	    void SurfaceCollisions( Thing* thing );
	    bool FloorCollisions( Thing* thing );
    int flag;
    protected:

	    int num;
	    bool adjoined;
	    Poly polygon;
	    Poly clipped;
	    Sector *adjoin;
	    Surface *mirror;
	    int flags;
	    int face;
	    int geo;
	    int adjnum;
	    int visible;
	    int adjoinFlags;
	    int drawCounter;
        
        vector<Cog::Script*> cogLinks;

	    float r;
	    float g;
	    float b;

	    int cel;

	    bool animate;
	    float animateFPS;
	    int animateFlags;
	    long animateTimer;

	    float slideU;
	    float slideV;
	    float offsetU;
	    float offsetV;
	    long slideTimer;
	    Math::Vector moveDelta;
	    Math::Vector moveVector;
    };
}
#endif