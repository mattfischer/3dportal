#ifndef R_MODEL_H
#define R_MODEL_H

#include "M_Vector.h"
#include "R_Vertex.h"
#include "W_Poly.h"

#include "JK_Key.h"

#include <string>

using std::string;

namespace Render
{
    struct Texture;
   
    class Model 
    {
    public:
        struct Face {
	        int type;
            World::Poly poly;
        };

        struct Mesh {
	        int numFaces;
	        int list;
	        string name;
	        Face *faces;
        };

        struct Node {
	        int mesh;
	        int numChildren;
	        Node *children;
	        Math::Vector position;
	        Math::Vector pivot;
	        Math::Vector rotation;
	        string name;
        };

        // JK_Model.cpp
	    Model( const string& filename );

	    // R_Model.cpp
        void Draw( float distance2, float light, Math::Vector tint, Jk::Key::Track *keyTrack );
	    float GetRadius();
	    Math::Vector GetInsertOffset();
	    Mesh *GetMesh( int g, int m );

    protected:
        struct GeoSet {
	        int numMeshes;
	        Mesh *meshes;
        };

	    int numGeoSets;
	    GeoSet *geoSets;
	    Node *rootNode;
	    Math::Vector insertOffset;
	    float radius;

        void DrawNode( Node *node, int g, float light, Math::Vector tint, Jk::Key::Track *keyTrack );
    };
}

#endif