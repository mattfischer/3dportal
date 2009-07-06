#include "JK_GOB.h"
#include "JK_Parse.h"
#include "JK_Level.h"

#include "R_Texture.h"
#include "R_Model.h"

#include "U_Lowercase.h"

extern char **textureFilenames;

struct TempHierarchyNode {
	int mesh;
	int parent;
	int child;
	int sibling;
	int numChildren;
	
	R_Node node;
	bool complete;
};

R_Model::R_Model( const string& filename )
{
	string fullname;
	string line;
	string data;
	char *dataPointer;
	int size;
	int error;
	int pos, pos2;
	int g, m, f, i, j;
	int v, t;
	M_Vector *vertices;
	M_Vector *normals;
	M_Vector normal;
	R_TextureVertex *textureVertices;
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
	
	name = filename;
	
	fullname = "3do\\" + name;
	JK_GOB_GetFile( fullname, &dataPointer, &size );
	data = dataPointer;
	delete[] dataPointer;

	pos = 0;

	JKP_FindString( data, pos, size, "SECTION: MODELRESOURCE", error );
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "MATERIALS", error );
	numMaterials = JKP_GetInt( line, pos2, error );
	textureNames.resize( numMaterials );
	
	for( i = 0 ; i < numMaterials ; i++ )
	{
		line = JKP_GetNonEmptyLine(data, pos, size, error);
		pos2 = 0;

		JKP_GetFloat( line, pos2, error );

		matName = U_Lowercase( JKP_GetString( line, pos2, error ) );
		textureNames[i] = matName;
	}

	JKP_FindString( data, pos, size, "SECTION: GEOMETRYDEF", error );
	
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "RADIUS", error );
	radius = JKP_GetFloat( line, pos2, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "INSERT OFFSET", error );
	insertOffset.x = JKP_GetFloat( line, pos2, error );
	insertOffset.y = JKP_GetFloat( line, pos2, error );
	insertOffset.z = JKP_GetFloat( line, pos2, error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "GEOSETS", error );
	numGeoSets = JKP_GetInt( line, pos2, error );
	geoSets = new R_GeoSet[numGeoSets];
	
	for( g = 0 ; g < numGeoSets ; g++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
	
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		JKP_MatchString( line, pos2, "MESHES", error );
		geoSets[g].numMeshes = JKP_GetInt( line, pos2, error );
		geoSets[g].meshes = new R_Mesh[geoSets[g].numMeshes];

		for( m = 0 ; m < geoSets[g].numMeshes ; m++ )
		{
			JKP_GetNonEmptyLine( data, pos, size, error );
			
			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
			JKP_MatchString( line, pos2, "NAME", error );
			geoSets[g].meshes[m].name = U_Lowercase( JKP_GetString( line, pos2, error ) );

			JKP_GetNonEmptyLine( data, pos, size, error );
			JKP_GetNonEmptyLine( data, pos, size, error );
			JKP_GetNonEmptyLine( data, pos, size, error );
			JKP_GetNonEmptyLine( data, pos, size, error );
			
			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
			JKP_MatchString( line, pos2, "VERTICES", error );
			numVertices = JKP_GetInt( line, pos2, error );
			vertices = new M_Vector[numVertices];
			normals = new M_Vector[numVertices];

			for( i = 0 ; i < numVertices ; i++ )
			{
				line = JKP_GetNonEmptyLine( data, pos, size, error );
				pos2 = 0;
				JKP_GetFloat( line, pos2, error );

				vertices[i].x = JKP_GetFloat( line, pos2, error );
				vertices[i].y = JKP_GetFloat( line, pos2, error );
				vertices[i].z = JKP_GetFloat( line, pos2, error );
			}

			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
			JKP_MatchString( line, pos2, "TEXTURE VERTICES", error );
			numTextureVertices = JKP_GetInt( line, pos2, error );
			textureVertices = new R_TextureVertex[numTextureVertices];

			for( i = 0 ; i < numTextureVertices ; i++ )
			{
				line = JKP_GetNonEmptyLine( data, pos, size, error );
				pos2 = 0;
				JKP_GetFloat( line, pos2, error );

				textureVertices[i].u = JKP_GetFloat( line, pos2, error );
				textureVertices[i].v = JKP_GetFloat( line, pos2, error );
			}

			line = JKP_GetNonEmptyLine( data, pos, size, error );

			for( i = 0 ; i < numVertices ; i++ )
			{
				line = JKP_GetNonEmptyLine( data, pos, size, error );
				pos2 = 0;
				JKP_GetFloat( line, pos2, error );

				normals[i].x = JKP_GetFloat( line, pos2, error );
				normals[i].y = JKP_GetFloat( line, pos2, error );
				normals[i].z = JKP_GetFloat( line, pos2, error );
			}

			line = JKP_GetNonEmptyLine( data, pos, size, error );
			pos2 = 0;
			JKP_MatchString( line, pos2, "FACES", error );
			geoSets[g].meshes[m].numFaces = JKP_GetInt( line, pos2, error );
			geoSets[g].meshes[m].faces = new R_Face[geoSets[g].meshes[m].numFaces];

			for( f = 0 ; f < geoSets[g].meshes[m].numFaces ; f++ )
			{
				line = JKP_GetNonEmptyLine( data, pos, size, error );
				pos2 = 0;
				JKP_GetFloat( line, pos2, error );

				index = JKP_GetInt( line, pos2, error );
				
				geoSets[g].meshes[m].faces[f].type = JKP_GetHex( line, pos2, error );
				JKP_GetFloat( line, pos2, error );
				JKP_GetFloat( line, pos2, error );
				JKP_GetFloat( line, pos2, error );
				JKP_GetFloat( line, pos2, error );

				numVertices = JKP_GetInt( line, pos2, error );
				geoSets[g].meshes[m].faces[f].poly = W_Poly( numVertices );

				if( index < 0 || index >= numMaterials )
					geoSets[g].meshes[m].faces[f].poly.SetTexture( NULL );
				else
					geoSets[g].meshes[m].faces[f].poly.SetTexture( currentLevel.textures[textureNames[index]] );
				
				for( i = 0 ; i < numVertices ; i++ )
				{
					v = JKP_GetInt( line, pos2, error );
					t = JKP_GetInt( line, pos2, error );
					geoSets[g].meshes[m].faces[f].poly[i].position = vertices[v];
					geoSets[g].meshes[m].faces[f].poly[i].normal = normals[v];
					geoSets[g].meshes[m].faces[f].poly[i].texture = textureVertices[t];
					//newModel->geoSets[g].meshes[m].faces[f].poly[i].texture.u/=newModel->geoSets[g].meshes[m].faces[f].poly.GetTexture()->sizeX;
					//newModel->geoSets[g].meshes[m].faces[f].poly[i].texture.v/=newModel->geoSets[g].meshes[m].faces[f].poly.GetTexture()->sizeY;
					geoSets[g].meshes[m].faces[f].poly[i].intensity = 0;
				}
			}

			line = JKP_GetNonEmptyLine( data, pos, size, error );

			for( f = 0 ; f < geoSets[g].meshes[m].numFaces ; f++ )
			{
				line = JKP_GetNonEmptyLine( data, pos, size, error );
				pos2 = 0;
				JKP_GetFloat( line, pos2, error );

				normal.x = JKP_GetFloat( line, pos2, error );
				normal.y = JKP_GetFloat( line, pos2, error );
				normal.z = JKP_GetFloat( line, pos2, error );
				geoSets[g].meshes[m].faces[f].poly.SetPlane( normal );
			}

			delete[] vertices;
			delete[] normals;
			delete[] textureVertices;
		}
	}

	JKP_FindString( data, pos, size, "SECTION: HIERARCHYDEF", error );
	
	line = JKP_GetNonEmptyLine( data, pos, size, error );
	pos2 = 0;
	JKP_MatchString( line, pos2, "HIERARCHY NODES", error );
	numHierarchyNodes = JKP_GetInt( line, pos2, error );
	nodes = new TempHierarchyNode[numHierarchyNodes];

	for( i = 0 ; i < numHierarchyNodes ; i++ )
	{
		line = JKP_GetNonEmptyLine( data, pos, size, error );
		pos2 = 0;
		JKP_GetFloat( line, pos2, error );

		JKP_GetHex( line, pos2, error );
		JKP_GetHex( line, pos2, error );

		nodes[i].mesh = JKP_GetInt( line, pos2, error );
		nodes[i].parent = JKP_GetInt( line, pos2, error );
		nodes[i].child = JKP_GetInt( line, pos2, error );
		nodes[i].sibling = JKP_GetInt( line, pos2, error );
		nodes[i].numChildren = JKP_GetInt( line, pos2, error );

		nodes[i].node.position.x = JKP_GetFloat( line, pos2, error );
		nodes[i].node.position.y = JKP_GetFloat( line, pos2, error );
		nodes[i].node.position.z = JKP_GetFloat( line, pos2, error );

		nodes[i].node.rotation.x = JKP_GetFloat( line, pos2, error );
		nodes[i].node.rotation.y = JKP_GetFloat( line, pos2, error );
		nodes[i].node.rotation.z = JKP_GetFloat( line, pos2, error );

		nodes[i].node.pivot.x = JKP_GetFloat( line, pos2, error );
		nodes[i].node.pivot.y = JKP_GetFloat( line, pos2, error );
		nodes[i].node.pivot.z = JKP_GetFloat( line, pos2, error );
		
		nodes[i].node.name = JKP_GetString( line, pos2, error );

		nodes[i].node.mesh = nodes[i].mesh;
		nodes[i].node.numChildren = nodes[i].numChildren;
		nodes[i].node.children = new R_Node[nodes[i].node.numChildren];
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

	rootNode = new R_Node;
	*rootNode = nodes[0].node;

	delete[] nodes;
}
