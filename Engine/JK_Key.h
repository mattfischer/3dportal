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
    M_Vector deltaPosition;
    M_Vector deltaOrientation;
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

	void interpolateFrame( const string& node, float time, int flags, M_Vector& position, M_Vector& orientation );

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

struct JK_Key_Instance
{
    JK_Key *key;
    float time;
    int flags;

    JK_Key_Instance() : key(NULL), time(0), flags(0) {}
    JK_Key_Instance(JK_Key *_key, float _time, int _flags) : key(_key), time(_time), flags(_flags) {}
};

#endif