#ifndef JK_LEVEL_H
#define JK_LEVEL_H

#include "Global.h"

#include "W_Sector.h"

#include "U_Collection.h"
#include "U_VectorMap.h"

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

using std::string;
using std::map;

class W_Sector;
class W_Thing;
class JK_Template;
class R_Model;
struct R_Texture;
class C_Script;
class S_Sound;
class S_SoundClass;
class JK_Colormap;
class W_Thing;
class JK_Key;
class JK_AnimClass;

using boost::shared_ptr;

struct JK_Level {
	int playerNum;

	U_VectorMap<R_Texture*> textures;
	U_VectorMap<JK_Colormap*> colormaps;
	U_VectorMap<C_Script*> cogScripts;
	U_VectorMap<S_Sound*> sounds;
	U_VectorMap<S_SoundClass*> soundClasses;
	U_VectorMap<R_Model*> models;
	U_VectorMap<JK_Key*> keyframes;
    U_VectorMap<JK_AnimClass*> animClasses;

	std::vector<shared_ptr<W_Thing> > things;
	std::vector<C_Script*> cogs;
	U_VectorMap<JK_Template*> templates;

	int numSurfaces;
	W_Surface *surfaces;

	int numSectors;
	W_Sector *sectors;

	float gravity;
	int numPixelsPerRev;
	float horizonDistance;
	float ceilingSkyZ;
	float horizonOffsetX;
	float horizonOffsetY;
	
	float LODDistances[4];
};

extern JK_Level currentLevel;

void JK_Level_Load( const string& name );
 
#endif