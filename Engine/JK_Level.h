#ifndef JK_LEVEL_H
#define JK_LEVEL_H

#include "Global.h"

#include "W_Sector.h"

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

        Util::VectorMap<R_Texture*> textures;
	    Util::VectorMap<JK_Colormap*> colormaps;
	    Util::VectorMap<C_Script*> cogScripts;
	    Util::VectorMap<S_Sound*> sounds;
	    Util::VectorMap<S_SoundClass*> soundClasses;
	    Util::VectorMap<R_Model*> models;
        Util::VectorMap<Jk::Key*> keyframes;
        Util::VectorMap<Jk::AnimClass*> animClasses;
	    Util::VectorMap<R_Sprite*> sprites;

	    std::vector<shared_ptr<W_Thing> > things;
	    std::vector<C_Script*> cogs;
        Util::VectorMap<Jk::Template*> templates;

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