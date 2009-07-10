#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#include "R_Model.h"
#include "R_Frame.h"
#include "R_OpenGL.h"

#include "W_Thing.h"
#include "W_Sector.h"

extern shared_ptr<W_Thing> player;

R_Model *povModel = NULL;
JK_Key_Instance povKeyInstance;

int ScreenX=1280;
int ScreenY=800;

float SX=tan(FOV/2*3.14159265359/180)*ScreenX/ScreenY+.1;
float SY=tan(FOV/2*3.14159265359/180)+.1;

M_Matrix perspectiveMatrix(1, 0 , 0, 0,
						 0 , 1, 0, 0,
						 0 , 0 , 1, 0,
						 0 , 0 , -1, 0);
M_Matrix rotationMatrix;
M_Matrix rotationInverseMatrix;

M_Matrix worldviewMatrix;
M_Matrix worldviewInverseMatrix;

M_Matrix coordConversionMatrix( 1 , 0 , 0, 0,
							  0 , 0 , 1, 0,
							  0 , -1, 0, 0,
							  0 , 0 , 0, 1
							 );
M_Matrix coordConversionInverseMatrix( 1 , 0 , 0, 0,
									 0 , 0 ,-1, 0,
									 0 , 1 , 0, 0,
									 0 , 0 , 0, 1
									);
M_Matrix totalTransformationMatrix;

int globalFlag=0;

R_Frustum R_WindowFrustum;

void R_Frame_ConstructMatricies()
{
	float xcos, xsin;
	float ycos, ysin;
	float zcos, zsin;

	M_Vector position, rotation;

	position=player->GetEyePosition();
	rotation=player->GetCompositeRotation();

	xcos=cos(rotation.y*PI/180);
	xsin=sin(rotation.y*PI/180);
	ycos=cos(rotation.x*PI/180);
	ysin=sin(rotation.x*PI/180);
	zcos=cos(rotation.z*PI/180);
	zsin=sin(rotation.z*PI/180);

	M_Matrix m(1 , 0 , 0, -position.x, 
		     0 , 1 , 0, -position.y,
			 0 , 0 , 1, -position.z,
			 0 , 0 , 0, 1
			 );
	M_Matrix m2(xcos,xsin,0,0,
			  -xsin,xcos,0,0,
			  0,0,1,0,
			  0,0,0,1
			  );
	
	M_Matrix m3(1,0,0,0,
			  0,ycos, ysin,0,
			  0, -ysin, ycos,0,
			  0,0,0,1);
	
	M_Matrix m4(zcos, 0, zsin,0,
				0,1,0,0,
				-zsin,0,zcos,0,
			    0,0,0,1);

	M_Matrix mi(1 , 0 , 0, position.x, 
		      0 , 1 , 0, position.y,
			  0 , 0 , 1, position.z,
			  0 , 0 , 0, 1
			 );
	M_Matrix mi2(xcos,-xsin,0,0,
			   xsin,xcos,0,0,
			   0,0,1,0,
			   0,0,0,1
			  );
	
	M_Matrix mi3(1,0,0,0,
			  0,ycos, -ysin,0,
			  0, ysin, ycos,0,
			  0,0,0,1);

	M_Matrix mi4(zcos, 0, -zsin,0,
				0,1,0,0,
				zsin,0,zcos,0,
			    0,0,0,1);

	worldviewMatrix=coordConversionMatrix*m4*m3*m2*m;
	worldviewInverseMatrix=mi*mi2*mi3*mi4*coordConversionInverseMatrix;
	totalTransformationMatrix=perspectiveMatrix*worldviewMatrix;
	rotationMatrix=coordConversionMatrix*m4*m3*m2;
	rotationInverseMatrix=mi2*mi3*mi4*coordConversionInverseMatrix;
}

void R_Frame_Render(float time)
{
	M_Vector position, rotation;
    
	position=player->GetEyePosition();
	rotation=player->GetCompositeRotation();

	glMatrixMode(GL_MODELVIEW);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glRotatef(-rotation.z,0,0,1);
	glRotatef(-rotation.x,1,0,0);
	glRotatef(-rotation.y,0,1,0);
	
	glTranslatef(-position.x, -position.z, position.y);
	R_Frame_ConstructMatricies();

	R_WindowFrustum.x0=-SX;
	R_WindowFrustum.x1=SX;
	R_WindowFrustum.y0=-SY;
	R_WindowFrustum.y1=SY;
	
	R_WindowFrustum.x00d=R_WindowFrustum.x01d=R_WindowFrustum.x0;
	R_WindowFrustum.x10d=R_WindowFrustum.x11d=R_WindowFrustum.x1;
	
	player->GetSector()->Draw(R_WindowFrustum,NULL);

	if( povModel )
	{
		glLoadIdentity();
		glTranslatef( 0, -.04, -.02 );
		
        if( povKeyInstance.key )
        {
            povKeyInstance.time += time;
        }

		povModel->Draw( 0, 1, M_Vector( 0, 0, 0 ), &povKeyInstance );
	}

	SwapBuffers(hDC);

	globalFlag++;
}
