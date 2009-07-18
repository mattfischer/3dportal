#include "R_Model.h"
#include "R_Texture.h"

#include "JK_Level.h"
#include "JK_Flags.h"

#include "G_Game.h"

#include <windows.h>
#include <gl/gl.h>

extern bool updateThings;
extern bool drawThings;
extern bool drawPolygons;

float R_Model::GetRadius()
{
	return radius;
}

Math::Vector R_Model::GetInsertOffset()
{
	return insertOffset;
}

void R_Model::Draw(float distance2, float light, Math::Vector tint, Jk::Key::Track *keyTrack )
{
	int LOD;
	int i;

	//if(!drawThings) return;

	if( G_GoodFramerate() ) LOD=0;
	else
	{
		LOD = numGeoSets - 1;
		for( i = 0; i < 4; i++ )
		{
			if( numGeoSets == i) break;
			if( distance2 < currentLevel.LODDistances[i] * currentLevel.LODDistances[i] )
			{
				LOD = i;
				break;
			}
		}
	}

	DrawNode( rootNode, LOD, light, tint, keyTrack );
}

void R_Model::DrawNode(R_Node *node, int g, float light, Math::Vector tint, Jk::Key::Track *keyTrack )
{
	int i;
	R_Mesh *mesh;
	Math::Vector position;
	Math::Vector rotation;

	if( keyTrack && keyTrack->key )
	{
		keyTrack->key->interpolateFrame( node->name, keyTrack->time, keyTrack->flags, position, rotation );
	}
    else
    {
        position = node->position;
        rotation = node->rotation;
    }

	mesh = &geoSets[g].meshes[node->mesh];
	glTranslatef( position.x, position.z, -( position.y ) );
	
    glRotatef( rotation.y, 0, 1, 0 );
    glRotatef( rotation.x, 1, 0, 0 );	    
    glRotatef( rotation.z, 0, 0, -1 );

	if( node->mesh != -1 )
	{
		glTranslatef( node->pivot.x, node->pivot.z, -node->pivot.y );
		
		for( i = 0 ; i < mesh->numFaces ; i++ )
			mesh->faces[i].poly.Draw( tint, light, mesh->faces[i].type&JK_FACE_TRANSLUCENT, !( mesh->faces[i].type & JK_FACE_TWO_SIDED ), 0, 0, 0 );

		glTranslatef( -node->pivot.x, -node->pivot.z, node->pivot.y );
	}
	

	for( i = 0 ; i < node->numChildren ; i++ )
		DrawNode( &node->children[i], g, light, tint, keyTrack );

    glRotatef( -( rotation.z ), 0, 0, -1 );
    glRotatef( -( rotation.x ), 1, 0, 0 );
    glRotatef( -( rotation.y ), 0, 1, 0 );
	
	glTranslatef( -( position.x ), -( position.z ), position.y );
}
/*
char *R_Model::GetName()
{
	return name;
}

void R_Model::SetNum(int n)
{

}*/

R_Mesh *R_Model::GetMesh(int g, int m)
{
	return &geoSets[g].meshes[m];
}