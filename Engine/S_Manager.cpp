#include "S_Manager.h"
#include "S_Sound.h"

#include "W_Thing.h"
#include "W_Sector.h"

#include "M_Vector.h"

#include "JK_Level.h"

#include <windows.h>
#include <dsound.h>
#include <math.h>

#include <vector>

enum S_PlayType { S_LOCAL, S_THING, S_POS };

struct S_PlayItem
{
	S_PlayType type;
	S_Sound *sound;
	float volume;
	float minDist;
	float maxDist;

	float localPan;

	int thing;

	M_Vector position;

	S_PlayItem *next;
};

LPDIRECTSOUND lpDS;
std::vector<S_PlayItem*> items;
S_Sound *sectorSound;
CRITICAL_SECTION critSec;
extern shared_ptr<W_Thing> player;

void S_Initialize( HWND hWnd )
{
	DirectSoundCreate( NULL, &lpDS, NULL );
	lpDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );

	InitializeCriticalSection( &critSec );
}


void S_Update()
{
	M_Vector distance, direction;
	float volume;
	float magnitude;
	M_Vector position;
	shared_ptr<W_Thing> thing;
	unsigned int i;

	EnterCriticalSection( &critSec );
	player->GetSector()->PlaySectorSound();

	for( i = 0 ; i < items.size() ; i++ )
	{
		switch( items[i]->type )
		{
		case S_THING:
		case S_POS:
			distance = items[i]->position - player->GetPosition();
			magnitude = distance.Magnitude();
			if( magnitude < items[i]->minDist ) volume = 1;
			else if( magnitude > items[i]->maxDist ) volume = 0;
			else volume = ( ( items[i]->maxDist - magnitude ) / ( items[i]->maxDist - items[i]->minDist ) );

			items[i]->sound->SetVolume( volume * items[i]->volume);

			direction = M_Vector( cos( player->GetCompositeRotation().y * 3.14 / 180 ) , sin( player->GetCompositeRotation().y * 3.14 / 180 ), 0 );
			distance.Normalize();
			items[i]->sound->SetPan( distance * direction * .8 * ( 1 - volume ) );
			
			if( items[i]->type == S_THING )
			{
				thing = currentLevel.things[items[i]->thing];
				if( thing )
				{
					items[i]->position = thing->GetPosition();
				}
			}
			break;
		}

		items[i]->sound->Update();	

		if( items[i]->sound->GetStatus() == S_STOPPED )
		{
			delete items[i]->sound;
			items.erase( items.begin() + i );
			i--;
		}
	}
	LeaveCriticalSection( &critSec );
}

void S_PlayLocal( S_Sound *sound, float volume, float pan )
{
	S_PlayItem *newItem;
	bool done;

	EnterCriticalSection( &critSec );

	newItem = new S_PlayItem;
	newItem->type = S_LOCAL;
	newItem->sound = new S_Sound( *sound );
	newItem->volume = volume;
	newItem->localPan = pan;

	newItem->sound->SetVolume( volume );
	newItem->sound->SetPan( pan );
	newItem->sound->Play( false );
	
	items.push_back( newItem );
	
	LeaveCriticalSection( &critSec );
}

S_Sound *S_PlayThing( S_Sound *sound, shared_ptr<W_Thing> thing, bool loop, float volume, float minDist, float maxDist )
{
	S_PlayItem *newItem;
	bool done;

	if( thing == NULL ) return NULL;
	if( sound == NULL ) return NULL;

	if( minDist < 0 ) minDist = .5;
	if( maxDist < 0 ) maxDist = 2.5;
	
	newItem = new S_PlayItem;
	newItem->type = S_THING;
	newItem->sound = new S_Sound( *sound );
	newItem->thing = thing->GetNum();
	newItem->position = thing->GetPosition();
	newItem->volume = volume;
	newItem->minDist = minDist;
	newItem->maxDist = maxDist;
	newItem->sound->Play( loop );
	
	EnterCriticalSection( &critSec );

	items.push_back( newItem );

	LeaveCriticalSection( &critSec );

	return newItem->sound;
}

void S_PlayPos( S_Sound *sound, M_Vector position, float volume, float minDist, float maxDist )
{
	S_PlayItem *newItem;
	bool done;

	if( minDist < 0 ) minDist = .5;
	if( maxDist < 0 ) maxDist = 2.5;

	newItem = new S_PlayItem;
	newItem->type = S_POS;
	newItem->minDist = minDist;
	newItem->maxDist = maxDist;
	newItem->sound = new S_Sound( *sound );
	newItem->position = position;
	newItem->volume = volume;
	newItem->sound->Play( false );
	
	EnterCriticalSection( &critSec );

	items.push_back( newItem );

	LeaveCriticalSection( &critSec );
}

void S_PlaySector( S_Sound *sound, float volume )
{
	if( sectorSound==NULL || sound==NULL || sectorSound->GetNum()!=sound->GetNum() )
	{
		if( sectorSound != NULL )
		{
			sectorSound->Stop();
			sectorSound->Update();
			delete sectorSound;
			sectorSound = NULL;
		}
				
		if( sound != NULL )
		{
			sectorSound = new S_Sound( *sound );
			sectorSound->SetVolume( volume );
			sectorSound->Play( true );
			sectorSound->Update();
		}
	}
	else if( sectorSound != NULL )
	{
		sectorSound->SetVolume( volume );
		sectorSound->Update();
	}
}