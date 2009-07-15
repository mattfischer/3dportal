#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "Global.h"

#include "G_Game.h"
#include "G_Inventory.h"

#include "R_Frame.h"
#include "R_Texture.h"
#include "R_OpenGL.h"

#include "W_Thing.h"
#include "W_Poly.h"

#include "I_Input.h"

#include "C_Script.h"

#include "JK_Level.h"
#include "JK_GOB.h"

#include "S_Manager.h"

shared_ptr<W_Thing> player;
extern R_Model *povModel;

#define NUMAVERAGEFRAMES 1
float lastFrameTimes[NUMAVERAGEFRAMES];
int lastFrameTimeIndex=0;

long fpsTimer;
int frames;

int fps=0;

LARGE_INTEGER timer_frequency;

int showFramerate=1;

void ShowFramerate(int framerate);

extern bool updateThings;

extern char levelName[];

bool G_GoodFramerate()
{
	if(fps>60) return true;
	else return false;
}

void G_Initialize()
{ 
	int i;
	
    Jk::Gob::init();
	
	C_Script::Setup();
	
	JK_Level_Load( string( levelName ) );
	for( i = 0 ; i < currentLevel.textures.size() ; i++ )
		currentLevel.textures[i]->Register( currentLevel.colormaps );

	//R_Texture_Register(currentLevel.textures, currentLevel.numTextures);

	G_SetupInventory();
	fpsTimer=GetTickCount();
	frames=0;
	for(i=0;i<NUMAVERAGEFRAMES;i++)
		lastFrameTimes[i]=.015;

	QueryPerformanceFrequency(&timer_frequency);

	for(i=0;i<currentLevel.cogs.size();i++)
		if(currentLevel.cogs[i])
			currentLevel.cogs[i]->Message("startup", -1, 0, -1);

    Jk::Gob::close();
}

void G_GameLoop()
{
	float time;
	float average;
	int i;

	LARGE_INTEGER frameBegin;
	LARGE_INTEGER frameEnd;
	LARGE_INTEGER timeElapsed;
	if(!OpenGLStarted) return;
	
	QueryPerformanceCounter(&frameBegin);
	
	average=0;
	for(i=0;i<NUMAVERAGEFRAMES;i++)
		average+=lastFrameTimes[i];
	average/=(float)NUMAVERAGEFRAMES;

	I_Process(average);
	W_Thing::UpdateThings(average);
	S_Update();
	R_Frame_Render(average);
	
	frames++;
	if(GetTickCount()>fpsTimer+1000)
	{
		fps=frames;
		frames=0;
		fpsTimer=GetTickCount();
		if(showFramerate) ShowFramerate(fps);
	}
		
	QueryPerformanceCounter(&frameEnd);
	
	timeElapsed.QuadPart=frameEnd.QuadPart-frameBegin.QuadPart;
	time=(float)timeElapsed.QuadPart/(float)timer_frequency.QuadPart;
	
	//if(average>time) Sleep((average-time)*1000.0);
	lastFrameTimes[lastFrameTimeIndex]=time;
	lastFrameTimeIndex++;
	if(lastFrameTimeIndex==NUMAVERAGEFRAMES) lastFrameTimeIndex=0;
}