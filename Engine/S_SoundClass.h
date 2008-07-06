#ifndef S_CLASS_H
#define S_CLASS_H

class S_Sound;
class W_Thing;

#include "U_Collection.h"

struct S_ClassEntry
{
	string name;
	S_Sound *sound;
	int flags;
	float minDist;
	float maxDist;
	float volume;
};

struct S_SoundClass : public U_NameableItem
{
public:
	S_SoundClass( const string& filename );

	S_Sound *Play( const string& sound, int thing );

protected:
	int numEntries;
	S_ClassEntry *entries;
};

#endif