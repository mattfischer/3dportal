#include "W_Sector.h"
#include "W_Surface.h"
#include "W_Thing.h"

#include "C_Script.h"

#include "S_Manager.h"

W_Sector::W_Sector()
{
	recurseGuard=false;
	renderAdjoins=true;

	InitializeCriticalSection(&critSec);
}

int W_Sector::NumSurfaces()
{
	return numSurfaces;
}

W_Surface *W_Sector::operator[](int n)
{
	return surfaces[n];
}

void W_Sector::AddThing(W_Thing* thing )
{
	EnterCriticalSection( &critSec );
	things.push_back( thing );
	LeaveCriticalSection( &critSec );
}

void W_Sector::RemoveThing( W_Thing* thing )
{
	int i;
	EnterCriticalSection( &critSec );
	for( i = 0 ; i < things.size() ; i++ )
	{
		if( things[i] == thing )
		{
			things.erase( things.begin() + i );
		}
	}
	LeaveCriticalSection( &critSec );
}

void W_Sector::AddCogLink(C_Script *cogScript)
{
	cogLinks.push_back( cogScript );
}

void W_Sector::SendCogMessages( const string& message, int source, bool synchronous)
{
	int i;
	for( i = 0 ; i < cogLinks.size() ; i++ )
	{
		cogLinks[i]->Message( message, num, 5, source, synchronous );
	}
}

int W_Sector::GetNum()
{
	return num;
}

void W_Sector::RenderAdjoins(bool r)
{
	renderAdjoins=r;
}

void W_Sector::SetLight(float l)
{
	extraLight=l;
	ambientLight=l;
}

void W_Sector::PlaySectorSound()
{
	S_PlaySector(sound, soundVolume);
}