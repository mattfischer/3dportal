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
class R_Model;
struct R_Texture;
class C_Script;
class S_Sound;
class S_SoundClass;
class JK_Colormap;
class W_Thing;
class R_Sprite;

using boost::shared_ptr;

namespace Jk
{
    class AnimClass;
    class Key;
    class Template;
    struct Level {
	    int playerNum;

	    U_VectorMap<R_Texture*> textures;
	    U_VectorMap<JK_Colormap*> colormaps;
	    U_VectorMap<C_Script*> cogScripts;
	    U_VectorMap<S_Sound*> sounds;
	    U_VectorMap<S_SoundClass*> soundClasses;
	    U_VectorMap<R_Model*> models;
        U_VectorMap<Jk::Key*> keyframes;
        U_VectorMap<Jk::AnimClass*> animClasses;
	    U_VectorMap<R_Sprite*> sprites;

	    std::vector<shared_ptr<W_Thing> > things;
	    std::vector<C_Script*> cogs;
        U_VectorMap<Jk::Template*> templates;

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
}

extern Jk::Level currentLevel;

void JK_Level_Load( const string& name );
 
#endif