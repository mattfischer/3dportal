#include "S_Manager.h"
#include "S_Sound.h"

#include "W_Thing.h"
#include "W_Sector.h"

#include "M_Vector.h"

#include "JK_Level.h"

#include <windows.h>
#include <dsound.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

#include <boost/weak_ptr.hpp>

extern shared_ptr<World::Thing> player;

namespace Sound
{
    enum PlayType { PLAY_LOCAL, PLAY_THING, PLAY_POS };

    struct PlayItem
    {
	    PlayType type;
	    Sound::Track *track;
	    float volume;
	    float minDist;
	    float maxDist;

	    float localPan;

        boost::weak_ptr<World::Thing> thing;

	    Math::Vector position;

	    PlayItem *next;
    };

    LPDIRECTSOUND lpDS;
    std::vector<PlayItem*> items;
    Sound::Track *sectorTrack;
    CRITICAL_SECTION critSec;

    void Initialize( HWND hWnd )
    {
	    DirectSoundCreate( NULL, &lpDS, NULL );
	    lpDS->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );

	    InitializeCriticalSection( &critSec );
    }


    void Update()
    {
	    Math::Vector distance, direction;
	    float volume;
	    float magnitude;
	    Math::Vector position;
        shared_ptr<World::Thing> thing;
	    unsigned int i;

	    EnterCriticalSection( &critSec );
	    player->GetSector()->PlaySectorSound();

	    for( i = 0 ; i < items.size() ; i++ )
	    {
		    switch( items[i]->type )
		    {
		    case PLAY_THING:
		    case PLAY_POS:
			    distance = items[i]->position - player->GetPosition();
			    magnitude = distance.Magnitude();
			    if( magnitude < items[i]->minDist ) volume = 1;
			    else if( magnitude > items[i]->maxDist ) volume = 0;
			    else volume = ( ( items[i]->maxDist - magnitude ) / ( items[i]->maxDist - items[i]->minDist ) );

			    items[i]->track->SetVolume( volume * items[i]->volume);

			    direction = Math::Vector( cos( player->GetCompositeRotation().y * M_PI / 180 ) , sin( player->GetCompositeRotation().y * M_PI / 180 ), 0 );
			    distance.Normalize();
			    items[i]->track->SetPan( distance * direction * .8 * ( 1 - volume ) );
    			
			    if( items[i]->type == PLAY_THING )
			    {
				    thing = items[i]->thing.lock();
				    if( thing )
				    {
					    items[i]->position = thing->GetPosition();
				    }
			    }
			    break;
		    }

		    items[i]->track->Update();	

            if( items[i]->track->GetStatus() == Sound::Track::STATE_STOPPED )
		    {
			    delete items[i]->track;
			    items.erase( items.begin() + i );
			    i--;
		    }
	    }
	    LeaveCriticalSection( &critSec );
    }

    void PlayLocal( Sound::Buffer *buffer, float volume, float pan )
    {
	    PlayItem *newItem;
	    bool done;

	    EnterCriticalSection( &critSec );

	    newItem = new PlayItem;
	    newItem->type = PLAY_LOCAL;
	    newItem->track = new Sound::Track( buffer );
	    newItem->volume = volume;
	    newItem->localPan = pan;

	    newItem->track->SetVolume( volume );
	    newItem->track->SetPan( pan );
	    newItem->track->Play( false );
    	
	    items.push_back( newItem );
    	
	    LeaveCriticalSection( &critSec );
    }

    Sound::Track *PlayThing( Sound::Buffer *buffer, shared_ptr<World::Thing> thing, bool loop, float volume, float minDist, float maxDist )
    {
	    PlayItem *newItem;
	    bool done;

	    if( thing == NULL ) return NULL;
	    if( buffer == NULL ) return NULL;

	    if( minDist < 0 ) minDist = .5;
	    if( maxDist < 0 ) maxDist = 2.5;
    	
	    newItem = new PlayItem;
	    newItem->type = PLAY_THING;
	    newItem->track = new Sound::Track( buffer );
	    newItem->thing = thing;
	    newItem->position = thing->GetPosition();
	    newItem->volume = volume;
	    newItem->minDist = minDist;
	    newItem->maxDist = maxDist;
	    newItem->track->Play( loop );
    	
	    EnterCriticalSection( &critSec );

	    items.push_back( newItem );

	    LeaveCriticalSection( &critSec );

	    return newItem->track;
    }

    void PlayPos( Sound::Buffer *buffer, Math::Vector position, float volume, float minDist, float maxDist )
    {
	    PlayItem *newItem;
	    bool done;

	    if( minDist < 0 ) minDist = .5;
	    if( maxDist < 0 ) maxDist = 2.5;

	    newItem = new PlayItem;
	    newItem->type = PLAY_POS;
	    newItem->minDist = minDist;
	    newItem->maxDist = maxDist;
	    newItem->track = new Sound::Track( buffer );
	    newItem->position = position;
	    newItem->volume = volume;
	    newItem->track->Play( false );
    	
	    EnterCriticalSection( &critSec );

	    items.push_back( newItem );

	    LeaveCriticalSection( &critSec );
    }

    void PlaySector( Sound::Buffer *buffer, float volume )
    {
	    if( sectorTrack == NULL || sectorTrack->buffer() != buffer )
	    {
		    if( sectorTrack != NULL )
		    {
			    sectorTrack->Stop();
			    sectorTrack->Update();
			    delete sectorTrack;
			    sectorTrack = NULL;
		    }
    				
		    if( buffer != NULL )
		    {
			    sectorTrack = new Sound::Track( buffer );
			    sectorTrack->SetVolume( volume );
			    sectorTrack->Play( true );
			    sectorTrack->Update();
		    }
	    }
	    else if( sectorTrack != NULL )
	    {
		    sectorTrack->SetVolume( volume );
		    sectorTrack->Update();
	    }
    }
}