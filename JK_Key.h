#ifndef JK_KEY_H
#define JK_KEY_H

#include "M_Vector.h"
#include <string>

using std::string;

namespace Jk
{
    class Key
    {
    public:
        struct Track
        {
            Key *key;
            float time;
            int flags;

            Track() : key(NULL), time(0), flags(0) {}
            Track(Key *_key, float _time, int _flags) : key(_key), time(_time), flags(_flags) {}
        };

	    Key( const string& filename );

	    void interpolateFrame( const string& node, float time, int flags, Math::Vector& position, Math::Vector& orientation );

    protected:
	    string name;

	    int flags;
	    int type;
	    int numFrames;
	    int fps;
	    int joints;

        struct Marker
        {
	        float frame;
	        int type;
        };

   	    int numMarkers;
	    Marker *markers;

        struct Frame
        {
	        int frame;
	        int flags;

	        Math::Vector position;
	        Math::Vector orientation;
            Math::Vector deltaPosition;
            Math::Vector deltaOrientation;
        };

        struct Node
        {
	        string name;
	        int numEntries;
	        Frame *entries;
        };

	    int numNodes;
	    Node *nodes;
    };
}

#endif