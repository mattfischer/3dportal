#ifndef R_MODEL_H
#define R_MODEL_H

#include "Global.h"

#include "M_Vector.h"
#include "R_Vertex.h"
#include "W_Poly.h"

#include "JK_Key.h"

#include <string>

struct R_Texture;

using std::string;

struct R_Face {
	int type;
	W_Poly poly;
};

struct R_Mesh {
	int numFaces;
	int list;
	string name;
	R_Face *faces;
};

struct R_GeoSet {
	int numMeshes;
	R_Mesh *meshes;
};

struct R_Node {
	int mesh;
	int numChildren;
	R_Node *children;
	M_Vector position;
	M_Vector pivot;
	M_Vector rotation;
	string name;
};

class R_Model 
{
public:
	// JK_Model.cpp
	R_Model( const string& filename );

	// R_Model.cpp
    void Draw( float distance2, float light, M_Vector tint, Jk::Key::Track *keyTrack );
	float GetRadius();
	M_Vector GetInsertOffset();
	R_Mesh *GetMesh( int g, int m );

protected:
	int numGeoSets;
	R_GeoSet *geoSets;
	R_Node *rootNode;
	M_Vector insertOffset;
	float radius;

    void DrawNode( R_Node *node, int g, float light, M_Vector tint, Jk::Key::Track *keyTrack );
};

void JK_Level_Load3DO( const string& name, R_Model *newModel );

#endif