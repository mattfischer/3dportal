#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "C_Script.h"

#include "S_Manager.h"

namespace World
{
    Sector::Sector()
    {
	    recurseGuard=false;
	    renderAdjoins=true;
    }

    int Sector::NumSurfaces()
    {
	    return numSurfaces;
    }

    Surface *Sector::operator[](int n)
    {
	    return surfaces[n];
    }

    void Sector::AddThing(Thing* thing )
    {
	    things.push_back( thing );
    }

    void Sector::RemoveThing( Thing* thing )
    {
	    int i;
	    for( i = 0 ; i < things.size() ; i++ )
	    {
		    if( things[i] == thing )
		    {
			    things.erase( things.begin() + i );
		    }
	    }
    }

    void Sector::AddCogLink(Cog::Script *cogScript)
    {
	    cogLinks.push_back( cogScript );
    }

    void Sector::SendCogMessages( const string& message, int source, bool synchronous)
    {
	    int i;
	    for( i = 0 ; i < cogLinks.size() ; i++ )
	    {
		    cogLinks[i]->Message( message, num, 5, source, synchronous );
	    }
    }

    int Sector::GetNum()
    {
	    return num;
    }

    void Sector::RenderAdjoins(bool r)
    {
	    renderAdjoins=r;
    }

    void Sector::SetLight(float l)
    {
	    extraLight=l;
	    ambientLight=l;
    }

    void Sector::PlaySectorSound()
    {
        Sound::PlaySector(sound, soundVolume);
    }
}