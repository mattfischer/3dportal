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

namespace Render
{
    class Model;
    struct Texture;
    class Sprite;
}
namespace Cog
{
    class Script;
}
namespace Sound
{
    class Buffer;
    class Class;
}

namespace World
{
    class Sector;
    class Thing;
    class Surface;
}

namespace Jk
{
    class Colormap;
}

using boost::shared_ptr;

namespace Jk
{
    class AnimClass;
    class Key;
    class Template;
    struct Level {
	    int playerNum;

        Util::VectorMap<Render::Texture*> textures;
        Util::VectorMap<Jk::Colormap*> colormaps;
        Util::VectorMap<Cog::Script*> cogScripts;
	    Util::VectorMap<Sound::Buffer*> sounds;
	    Util::VectorMap<Sound::Class*> soundClasses;
	    Util::VectorMap<Render::Model*> models;
        Util::VectorMap<Jk::Key*> keyframes;
        Util::VectorMap<Jk::AnimClass*> animClasses;
	    Util::VectorMap<Render::Sprite*> sprites;

        std::vector<shared_ptr<World::Thing> > things;
        std::vector<Cog::Script*> cogs;
        Util::VectorMap<Jk::Template*> templates;

	    int numSurfaces;
        World::Surface *surfaces;

	    int numSectors;
	    World::Sector *sectors;

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