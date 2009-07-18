#include "C_Script.h"

#include "I_Input.h"

#include "W_Thing.h"

#include "R_Frame.h"

#include "G_Console.h"
#include "G_Inventory.h"

#include "JK_Level.h"
#include "JK_Template.h"

#include <windows.h>
#include <math.h>
#include <Dinput.h>

extern shared_ptr<W_Thing> player;

#define KEY_DOWN(i) ((GetAsyncKeyState(i) & 0x8000) ? 1 : 0)
#define KEY_UP(i) ((GetAsyncKeyState(i) & 0x8000) ? 0 : 1)

bool inputEnabled=true;
bool doThingCollisions=true;
bool updateThings=true;
bool fullLight=false;
bool drawThings=true;
bool drawPolygons=true;

void QuitGame();

extern int curWeapon;

LPDIRECTINPUT         lpdi;         // DirectInput interface
LPDIRECTINPUTDEVICE   lpdiMouse;    // mouse device interface

void I_Setup(HWND hWnd, HINSTANCE hInst)
{
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&lpdi, NULL);
       
// We'll skip the enumeration step, since we care only about the
//   standard system mouse.

	lpdi->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL);
    
	lpdiMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE);
    
  // Note: c_dfDIMouse is an external DIDATAFORMAT structure supplied
  // by DirectInput.
  lpdiMouse->SetDataFormat(&c_dfDIMouse);
  
  lpdiMouse->Acquire();
}

void I_EnableInput(bool enable)
{
	inputEnabled=enable;
	if(inputEnabled)
		ShowCursor(true);
	else
	{
		ShowCursor(false);
		SetCursorPos(ScreenX/2, ScreenY/2);
	}
}

bool I_IsInputEnabled()
{
	return inputEnabled;
}

void I_ProcessMouse(float time)
{
	DIMOUSESTATE diMouseState;
	POINT cursorPoint;
	float mouseAnglesize=20;
	Math::Vector rotation;
	float x, y;
	float angleClamp=20;
    static int lastInput=0;
	static LONG controlTimer = 0;

    if(GetTickCount() - lastInput < 10) return;
    
    time = (GetTickCount() - lastInput) / 1000.0;
    lastInput = GetTickCount();

	if(KEY_DOWN(VK_RBUTTON)) //thrust+=Math::Vector(0, 0, 6);
		//position.z+=stepsize*time;
		player->Jump();

	if(KEY_DOWN(VK_LBUTTON))
	{
 		if(GetTickCount() > controlTimer + 500)
		{
			if(curWeapon != -1)
				G_Items[curWeapon].cog->Message("fire", 0, 0, player->GetNum());
			controlTimer = GetTickCount();
		}
	}

	
	lpdiMouse->GetDeviceState(sizeof(diMouseState), &diMouseState);
       
//	GetCursorPos(&cursorPoint);

	rotation=player->GetRotation();

	x=-diMouseState.lY*mouseAnglesize*time;
	y=-diMouseState.lX*mouseAnglesize*time;

	/*
	if(cursorPoint.x-ScreenX/2>0)
		y=-(cursorPoint.x-ScreenX/2)*mouseAnglesize*time;
	if(cursorPoint.x-ScreenX/2<0)
		y=(ScreenX/2-cursorPoint.x)*mouseAnglesize*time;

	if(cursorPoint.y-ScreenY/2>0)
		x=-(cursorPoint.y-ScreenY/2)*mouseAnglesize*time;
	if(cursorPoint.y-ScreenY/2<0)
		x=(ScreenY/2-cursorPoint.y)*mouseAnglesize*time;
	*/
	if(x>angleClamp) x=angleClamp;
	if(x<-angleClamp) x=-angleClamp;
	if(y>angleClamp) y=angleClamp;
	if(y<-angleClamp) y=-angleClamp;

	rotation.x+=x;
	rotation.y+=y;

	if(rotation.x>70) rotation.x=70;
	if(rotation.x<-70) rotation.x=-70;
	if(rotation.y>360) rotation.y-=360;
	if(rotation.y<0) rotation.y+=360;

	player->SetRotation(rotation);
	SetCursorPos(ScreenX/2, ScreenY/2);
}

void I_ProcessKeyboard( float time )
{
	float anglesize = 150;
	float stepsize = 1.25;
	Math::Vector position, rotation;
	Math::Vector thrust;
	float angleSin, angleCos;
	static LONG controlTimer = 0;
	static bool activateDown = false;
	int i;

	position = player->GetPosition();
	rotation = player->GetCompositeRotation();

	angleSin = sin( rotation.y * 3.14 / 180 );
	angleCos = cos( rotation.y * 3.14 / 180 );

	thrust = Math::Vector( 0, 0, 0 );

	if( KEY_DOWN( VK_RIGHT ) ) 
	{
		thrust += Math::Vector( angleCos, angleSin, 0 );
		thrust = thrust * player->GetMaxThrust() * .75;
		//position.x+=cos(rotation.y*3.14/180)*stepsize*time;
		//position.y+=sin(rotation.y*3.14/180)*stepsize*time;
	}
	
	if( KEY_DOWN( VK_LEFT ) )
	{ 
		thrust += Math::Vector( -angleCos, -angleSin, 0 ); 
		thrust = thrust * player->GetMaxThrust() * .75;
		//position.x-=cos(rotation.y*3.14/180)*stepsize*time;
		//position.y-=sin(rotation.y*3.14/180)*stepsize*time;
	}

	if( KEY_DOWN( VK_UP ) )
	{
		thrust += Math::Vector( -angleSin, angleCos, 0 );
		thrust = thrust * player->GetMaxThrust();
		//position.x-=sin(rotation.y*3.14/180)*stepsize*time;
		//position.y+=cos(rotation.y*3.14/180)*stepsize*time;
	}
	
	if( KEY_DOWN( VK_DOWN ) )
	{
		thrust += Math::Vector( angleSin, -angleCos, 0 );
		thrust = thrust * player->GetMaxThrust() * .5;
		//position.x+=sin(rotation.y*3.14/180)*stepsize*time;
		//position.y-=cos(rotation.y*3.14/180)*stepsize*time;
	}
	
	if( KEY_DOWN( 'C' ) ) player->SetCrouched( true );
	else			      player->SetCrouched( false );

	if( KEY_DOWN( 'T' ) ) doThingCollisions = false;
	else			      doThingCollisions = true;
	
	if( KEY_DOWN('U' ) ) updateThings = false;
	else			     updateThings = true;

	if( KEY_DOWN( 'L' ) ) fullLight = true;
	else			      fullLight = false;

	if( KEY_DOWN( 'D' ) ) drawThings = false;
	else			      drawThings = true;

	if( KEY_DOWN( 'P' ) ) drawPolygons = false;
	else				  drawPolygons = true;
	
	if( GetTickCount() > controlTimer + 500 )
	{
		if( KEY_DOWN( VK_F12 ) )
		{
			CON_ToggleVisible();
			controlTimer = GetTickCount();
		}
		if( KEY_DOWN( VK_F2 ) ) 
		{
			for( i = 1 ; i < currentLevel.things.size() ; i++ )
				if( currentLevel.things[( i + player->GetNum() ) % currentLevel.things.size()]->GetTemplate()->GetName() == "walkplayer" )
				{
					player = currentLevel.things[( i+player->GetNum() ) % currentLevel.things.size()];
					break;
				}
			controlTimer = GetTickCount();
		}
		if( KEY_DOWN( VK_F1 ) ) 
		{
			for( i = currentLevel.things.size() - 2 ; i >=0 ; i-- )
				if( currentLevel.things[( i + player->GetNum() ) % currentLevel.things.size()]->GetTemplate()->GetName() == "walkplayer" )
				{
					player = currentLevel.things[( i + player->GetNum() ) % currentLevel.things.size()];
					break;
				}
			controlTimer = GetTickCount();
		}

        for(int i=0; i<=9; i++)
        {
            if( KEY_DOWN( '0' + i ) )
            {
                int bin = i;
                if( bin == 0 ) bin = 10;

                static int activeBin = 0;
                if( activeBin != bin )
                {
                    if( activeBin != 0 )
                    {
                        G_DeactivateBin( activeBin );
                    }
                    
                    G_ActivateBin( bin );
                    activeBin = bin;

                    controlTimer = GetTickCount();
                }
            }
        }
	}
	
	if( KEY_DOWN( VK_SPACE ) )
	{
		if( !activateDown )
		{
			activateDown = true;
			player->Activate();
		}
	}
	if( KEY_UP( VK_SPACE ) ) activateDown = false;
	if( KEY_DOWN( VK_ESCAPE ) ) QuitGame();

	if( KEY_DOWN( 'B' ) )
		activateDown = activateDown;

	/*if(thrust.Magnitude()>0)
	{
		thrust.Normalize();
		thrust*=player->GetMaxThrust();
		player->ApplyThrust(thrust);
	}*/
	
	player->ApplyThrust( thrust );

	/*if(KEY_DOWN('Q')) glEnable(GL_TEXTURE_2D);
	if(KEY_DOWN('W')) glDisable(GL_TEXTURE_2D);

	if(KEY_DOWN('E')) fullLight=0;
	if(KEY_DOWN('R')) fullLight=1;

	if(KEY_DOWN('Y')) diagClip=0;
	if(KEY_DOWN('U')) diagClip=1;

	if(KEY_DOWN('O')) glEnable(GL_DEPTH_TEST);
	if(KEY_DOWN('P')) glDisable(GL_DEPTH_TEST);*/

	//player->SetPosition(position);
}

void I_Process(float time)
{
	if(inputEnabled)
	{
		I_ProcessKeyboard(time);
		I_ProcessMouse(time);
	}
}
