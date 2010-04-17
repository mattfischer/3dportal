#ifndef S_SOUND_H
#define S_SOUND_H

#include <string>

#include <dsound.h>

using std::string;

namespace Sound
{
    class Buffer
    {
    public:
	    Buffer( const string& filename );
	    ~Buffer();

        void duplicateBuffer(LPDIRECTSOUNDBUFFER *buffer);

    protected:
	    LPDIRECTSOUNDBUFFER lpDSB;
    };

    class Track
    {
    public:
        Track( Buffer *s );
        ~Track();

        Buffer *buffer();

        void Play( bool loop );
	    void Stop();
	    void SetVolume( float v );
	    void SetPan( float p );
	    void Update();

        enum PlayState 
        { 
            STATE_EMPTY, 
            STATE_READY, 
            STATE_PENDING_START, 
            STATE_PLAYING, 
            STATE_PENDING_STOP, 
            STATE_STOPPED 
        };
	    PlayState GetStatus();

    protected:
        Buffer *buf;
        LPDIRECTSOUNDBUFFER lpDSB;
        float volume;
	    float pan;
	    bool loop;
	    PlayState status;
    };
}

#endif