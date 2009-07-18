#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#include "R_Model.h"
#include "R_Frame.h"
#include "R_OpenGL.h"

#include "W_Thing.h"
#include "W_Sector.h"

#include "U_Matrix.h"

extern shared_ptr<W_Thing> player;
int globalFlag=0;

namespace Render
{
    Model *povModel = NULL;
    Jk::Key::Track povKeyTrack;

    int ScreenX=1280;
    int ScreenY=800;

    float SX=tan(FOV/2*3.14159265359/180)*ScreenX/ScreenY+.1;
    float SY=tan(FOV/2*3.14159265359/180)+.1;

    Math::Matrix perspectiveMatrix(1, 0 , 0, 0,
						     0 , 1, 0, 0,
						     0 , 0 , 1, 0,
						     0 , 0 , -1, 0);
    Math::Matrix rotationMatrix;
    Math::Matrix rotationInverseMatrix;

    Math::Matrix worldviewMatrix;
    Math::Matrix worldviewInverseMatrix;

    Math::Matrix coordConversionMatrix( 1 , 0 , 0, 0,
							      0 , 0 , 1, 0,
							      0 , -1, 0, 0,
							      0 , 0 , 0, 1
							     );
    Math::Matrix coordConversionInverseMatrix( 1 , 0 , 0, 0,
									     0 , 0 ,-1, 0,
									     0 , 1 , 0, 0,
									     0 , 0 , 0, 1
									    );
    Math::Matrix totalTransformationMatrix;

    Frustum WindowFrustum;

    void Frame_ConstructMatricies()
    {
	    float xcos, xsin;
	    float ycos, ysin;
	    float zcos, zsin;

	    Math::Vector position, rotation;

	    position=player->GetEyePosition();
	    rotation=player->GetCompositeRotation();

	    xcos=cos(rotation.y*PI/180);
	    xsin=sin(rotation.y*PI/180);
	    ycos=cos(rotation.x*PI/180);
	    ysin=sin(rotation.x*PI/180);
	    zcos=cos(rotation.z*PI/180);
	    zsin=sin(rotation.z*PI/180);

        Math::Matrix m = Util::Matrix::Translate(-position);
	    Math::Matrix m2 = Util::Matrix::RotateZ(-rotation.y);
        Math::Matrix m3 = Util::Matrix::RotateX(-rotation.x);
        Math::Matrix m4 = Util::Matrix::RotateY(rotation.z);

        Math::Matrix mi = Util::Matrix::Translate(position);
        Math::Matrix mi2 = Util::Matrix::RotateZ(rotation.y);
        Math::Matrix mi3 = Util::Matrix::RotateX(rotation.x);
        Math::Matrix mi4 = Util::Matrix::RotateY(-rotation.z);

	    worldviewMatrix=coordConversionMatrix*m4*m3*m2*m;
	    worldviewInverseMatrix=mi*mi2*mi3*mi4*coordConversionInverseMatrix;
	    totalTransformationMatrix=perspectiveMatrix*worldviewMatrix;
	    rotationMatrix=coordConversionMatrix*m4*m3*m2;
	    rotationInverseMatrix=mi2*mi3*mi4*coordConversionInverseMatrix;
    }

    void Frame_Render(float time)
    {
	    Math::Vector position, rotation;
        
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
	    Frame_ConstructMatricies();

	    WindowFrustum.x0=-SX;
	    WindowFrustum.x1=SX;
	    WindowFrustum.y0=-SY;
	    WindowFrustum.y1=SY;
    	
	    WindowFrustum.x00d=WindowFrustum.x01d=WindowFrustum.x0;
	    WindowFrustum.x10d=WindowFrustum.x11d=WindowFrustum.x1;
    	
	    player->GetSector()->Draw(WindowFrustum,NULL);

	    if( povModel )
	    {
		    glLoadIdentity();
		    glTranslatef( 0, -.04, -.02 );
    		
            if( povKeyTrack.key )
            {
                povKeyTrack.time += time;
            }

		    povModel->Draw( 0, 1, Math::Vector( 0, 0, 0 ), &povKeyTrack );
	    }

	    SwapBuffers(hDC);

	    globalFlag++;
    }
}