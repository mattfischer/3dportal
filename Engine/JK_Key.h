#ifndef JK_KEY_H
#define JK_KEY_H

#include "M_Vector.h"
#include <string>

using std::string;

struct JK_K_Frame
{
	int frame;
	int flags;

	M_Vector position;
	M_Vector orientation;
};

struct JK_K_Node
{
	string name;
	int numEntries;
	JK_K_Frame *entries;
};

struct JK_K_Marker
{
	float frame;
	int type;
};

class JK_Key
{
public:
	JK_Key( const string& filename );

	void interpolateFrame( const string& node, float time, M_Vector& position, M_Vector& orientation );

protected:
	string name;

	int flags;
	int type;
	int numFrames;
	int fps;
	int joints;

	int numMarkers;
	JK_K_Marker *markers;

	int numNodes;
	JK_K_Node *nodes;
};

#endif