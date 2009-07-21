#include "JK_GOB.h"
#include "JK_Parse.h"
#include "JK_Level.h"

#include "R_Texture.h"
#include "R_Model.h"

#include "U_Lowercase.h"

#include <vector>

extern char **textureFilenames;

using std::vector;

namespace Render
{
    struct TempHierarchyNode {
	    int mesh;
	    int parent;
	    int child;
	    int sibling;
	    int numChildren;
    	
        Model::Node node;
	    bool complete;
    };

    Model::Model( const string& filename )
    {
	    string fullname;
        Jk::Parser::Line line;
	    string data;
	    bool error;
	    int g, m, f, i, j;
	    int v, t;
	    Math::Vector *vertices;
	    Math::Vector *normals;
	    Math::Vector normal;
	    TextureVertex *textureVertices;
	    int numVertices;
	    int numTextureVertices;
	    int numHierarchyNodes;
	    TempHierarchyNode *nodes;
	    bool done;
	    bool complete;
	    int cursor;
	    string matName;
	    int numMaterials;
	    vector<string> textureNames;
	    int index;

	    fullname = "3do\\" + filename;
        data = Jk::Gob::getFile( fullname );
        Jk::Parser parser(data);

	    parser.findString( "SECTION: MODELRESOURCE", error );
	    line = parser.getLine( error );
	    line.matchString( "MATERIALS", error );
	    numMaterials = line.getInt( error );
	    textureNames.resize( numMaterials );
    	
	    for( i = 0 ; i < numMaterials ; i++ )
	    {
		    line = parser.getLine( error);

		    line.getFloat( error );

            matName = Util::Lowercase( line.getString( error ) );
		    textureNames[i] = matName;
	    }

	    parser.findString( "SECTION: GEOMETRYDEF", error );

	    line = parser.getLine( error );
	    line.matchString( "RADIUS", error );
	    radius = line.getFloat( error );
    	
	    line = parser.getLine( error );
	    line.matchString( "INSERT OFFSET", error );
	    insertOffset.x = line.getFloat( error );
	    insertOffset.y = line.getFloat( error );
	    insertOffset.z = line.getFloat( error );
    	
	    line = parser.getLine( error );
	    line.matchString( "GEOSETS", error );
	    numGeoSets = line.getInt( error );
	    geoSets = new GeoSet[numGeoSets];
    	
	    for( g = 0 ; g < numGeoSets ; g++ )
	    {
		    line = parser.getLine( error );
    	
		    line = parser.getLine( error );
		    line.matchString( "MESHES", error );
		    geoSets[g].numMeshes = line.getInt( error );
		    geoSets[g].meshes = new Mesh[geoSets[g].numMeshes];

		    for( m = 0 ; m < geoSets[g].numMeshes ; m++ )
		    {
			    parser.getLine( error );
    			
			    line = parser.getLine( error );
			    line.matchString( "NAME", error );
			    geoSets[g].meshes[m].name = Util::Lowercase( line.getString( error ) );

			    parser.getLine( error );
			    parser.getLine( error );
			    parser.getLine( error );
			    parser.getLine( error );
    			
			    line = parser.getLine( error );
			    line.matchString( "VERTICES", error );
			    numVertices = line.getInt( error );
			    vertices = new Math::Vector[numVertices];
			    normals = new Math::Vector[numVertices];

			    for( i = 0 ; i < numVertices ; i++ )
			    {
				    line = parser.getLine( error );
				    line.getFloat( error );

				    vertices[i].x = line.getFloat( error );
				    vertices[i].y = line.getFloat( error );
				    vertices[i].z = line.getFloat( error );
			    }

			    line = parser.getLine( error );
			    line.matchString( "TEXTURE VERTICES", error );
			    numTextureVertices = line.getInt( error );
			    textureVertices = new TextureVertex[numTextureVertices];

			    for( i = 0 ; i < numTextureVertices ; i++ )
			    {
				    line = parser.getLine( error );

				    line.getFloat( error );

				    textureVertices[i].u = line.getFloat( error );
				    textureVertices[i].v = line.getFloat( error );
			    }

			    line = parser.getLine( error );

			    for( i = 0 ; i < numVertices ; i++ )
			    {
				    line = parser.getLine( error );

				    line.getFloat( error );

				    normals[i].x = line.getFloat( error );
				    normals[i].y = line.getFloat( error );
				    normals[i].z = line.getFloat( error );
			    }

			    line = parser.getLine( error );
                line.matchString( "FACES", error );
			    geoSets[g].meshes[m].numFaces = line.getInt( error );
			    geoSets[g].meshes[m].faces = new Face[geoSets[g].meshes[m].numFaces];

			    for( f = 0 ; f < geoSets[g].meshes[m].numFaces ; f++ )
			    {
				    line = parser.getLine( error );

                    line.getFloat( error );

				    index = line.getInt( error );
    				
				    geoSets[g].meshes[m].faces[f].type = line.getHex( error );
				    line.getFloat( error );
				    line.getFloat( error );
				    line.getFloat( error );
				    line.getFloat( error );

				    numVertices = line.getInt( error );
                    geoSets[g].meshes[m].faces[f].poly = World::Poly( numVertices );

				    if( index < 0 || index >= numMaterials )
					    geoSets[g].meshes[m].faces[f].poly.SetTexture( NULL );
				    else
                    {
                        string filename = textureNames[index];
                        int texIndex = currentLevel.textures.index( filename );
                        if( texIndex == -1 )
                        {
                            Texture *newTexture = new Texture( filename );
                            newTexture->Register( currentLevel.colormaps );
                            currentLevel.textures.push_back( newTexture, filename );
                            texIndex = currentLevel.textures.index( filename );
                        }
					    geoSets[g].meshes[m].faces[f].poly.SetTexture( currentLevel.textures[texIndex] );
                    }
    				
				    for( i = 0 ; i < numVertices ; i++ )
				    {
					    v = line.getInt( error );
					    t = line.getInt( error );
					    geoSets[g].meshes[m].faces[f].poly[i].position = vertices[v];
					    geoSets[g].meshes[m].faces[f].poly[i].normal = normals[v];
					    geoSets[g].meshes[m].faces[f].poly[i].texture = textureVertices[t];
					    //newModel->geoSets[g].meshes[m].faces[f].poly[i].texture.u/=newModel->geoSets[g].meshes[m].faces[f].poly.GetTexture()->sizeX;
					    //newModel->geoSets[g].meshes[m].faces[f].poly[i].texture.v/=newModel->geoSets[g].meshes[m].faces[f].poly.GetTexture()->sizeY;
					    geoSets[g].meshes[m].faces[f].poly[i].intensity = 0;
				    }
			    }

			    line = parser.getLine( error );

			    for( f = 0 ; f < geoSets[g].meshes[m].numFaces ; f++ )
			    {
				    line = parser.getLine( error );

                    line.getFloat( error );

				    normal.x = line.getFloat( error );
				    normal.y = line.getFloat( error );
				    normal.z = line.getFloat( error );
				    geoSets[g].meshes[m].faces[f].poly.SetPlane( normal );
			    }

			    delete[] vertices;
			    delete[] normals;
			    delete[] textureVertices;
		    }
	    }

	    parser.findString( "SECTION: HIERARCHYDEF", error );
    	
	    line = parser.getLine( error );
	    line.matchString( "HIERARCHY NODES", error );
	    numHierarchyNodes = line.getInt( error );
	    nodes = new TempHierarchyNode[numHierarchyNodes];

	    for( i = 0 ; i < numHierarchyNodes ; i++ )
	    {
		    line = parser.getLine( error );

		    line.getFloat( error );

		    line.getHex( error );
		    line.getHex( error );

		    nodes[i].mesh = line.getInt( error );
		    nodes[i].parent = line.getInt( error );
		    nodes[i].child = line.getInt( error );
		    nodes[i].sibling = line.getInt( error );
		    nodes[i].numChildren = line.getInt( error );

		    nodes[i].node.position.x = line.getFloat( error );
		    nodes[i].node.position.y = line.getFloat( error );
		    nodes[i].node.position.z = line.getFloat( error );

		    nodes[i].node.rotation.x = line.getFloat( error );
		    nodes[i].node.rotation.y = line.getFloat( error );
		    nodes[i].node.rotation.z = line.getFloat( error );

		    nodes[i].node.pivot.x = line.getFloat( error );
		    nodes[i].node.pivot.y = line.getFloat( error );
		    nodes[i].node.pivot.z = line.getFloat( error );
    		
		    nodes[i].node.name = Util::Lowercase( line.getString( error ) );

		    nodes[i].node.mesh = nodes[i].mesh;
		    nodes[i].node.numChildren = nodes[i].numChildren;
		    nodes[i].node.children = new Node[nodes[i].node.numChildren];
		    nodes[i].complete = 0;
	    }

	    done = false;
	    while( !done )
	    {
		    done = true;
		    for( i  = 0 ; i < numHierarchyNodes ; i++ )
		    {
			    if( nodes[i].complete ) continue;
    			
			    if( nodes[i].numChildren == 0 )
			    {
				    nodes[i].complete = true;
				    done = false;
			    }
			    else
			    {
				    cursor = nodes[i].child;
				    complete = true;
				    for( j = 0 ; j < nodes[i].numChildren ; j++ )
				    {
					    if( !nodes[cursor].complete )
					    {
						    complete = false;
						    break;
					    }
					    cursor = nodes[cursor].sibling;
				    }
				    if( complete )
				    {
					    cursor = nodes[i].child;
					    for( j = 0 ; j < nodes[i].node.numChildren ; j++ )
					    {
						    nodes[i].node.children[j] = nodes[cursor].node;
						    cursor = nodes[cursor].sibling;
					    }
					    nodes[i].complete = true;
					    done = false;
				    }
			    }
		    }
	    }

	    rootNode = new Node;
	    *rootNode = nodes[0].node;

	    delete[] nodes;
    }
}