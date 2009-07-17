#ifndef S_CLASS_H
#define S_CLASS_H

#include <string>

class S_Sound;
class S_SoundInstance;
class W_Thing;

using std::string;

struct S_ClassEntry
{
	string name;
	S_Sound *sound;
	int flags;
	float minDist;
	float maxDist;
	float volume;
};

struct S_SoundClass
{
public:
	S_SoundClass( const string& filename );

	S_SoundInstance *Play( const string& sound, int thing );

protected:
	int numEntries;
	S_ClassEntry *entries;
};

#endif