#ifndef W_SURFACE_H
#define W_SURFACE_H

#include "Global.h"

#include "W_Poly.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;

class W_Sector;
struct R_Frustum;
class C_Script;
class W_Thing;

class W_Surface {
	friend void JK_Level_Load( const string& name);

public:
	W_Surface();

	bool Adjoined();
	int GetFlags();
	int GetGeo();
	int GetFace();
	int GetAdjoinFlags();

	W_Poly GetPoly();
	W_Sector *GetAdjoin();
	W_Surface *GetMirror();

	void AddCogLink( C_Script *cogScript );
	void SendCogMessages( const string& message, int source, bool synchronous=false);

	void SetCel( int c );
	int GetCel();

	void Animate( float fps, int flags );
	void StopAnimate();
	void Slide( M_Vector direction, float speed );

	void Update();

	M_Vector GetMoveDelta();

	// R_Surface.cpp
	void Draw( M_Vector tint, R_Frustum frustum, float light );

	// P_Surface.cpp
	bool PerformActivate( M_Vector position, M_Vector point );
	void SurfaceCollisions( W_Thing* thing );
	bool FloorCollisions( W_Thing* thing );

protected:

	int num;
	bool adjoined;
	W_Poly polygon;
	W_Poly clipped;
	W_Sector *adjoin;
	W_Surface *mirror;
	int flags;
	int face;
	int geo;
	int adjnum;
	int visible;
	int adjoinFlags;
	int drawCounter;

	vector<C_Script*> cogLinks;

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
	M_Vector moveDelta;
	M_Vector moveVector;
};

#endif