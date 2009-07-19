#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "R_Model.h"
#include "R_Frame.h"
#include "R_OpenGL.h"

#include "W_Thing.h"
#include "W_Sector.h"

#include "U_Matrix.h"

extern shared_ptr<World::Thing> player;
int globalFlag=0;

namespace Render
{
    Model *povModel = NULL;
    Jk::Key::Track povKeyTrack;

    int Frame::ScreenX=1280;
    int Frame::ScreenY=800;

    float Frame::SX=tan(FOV/2*M_PI/180)*ScreenX/ScreenY+.1;
    float Frame::SY=tan(FOV/2*M_PI/180)+.1;

    Math::Matrix Frame::perspectiveMatrix(1, 0 , 0, 0,
						     0 , 0, 1, 0,
						     0 , -1 , 0, 0,
						     0 , 1 , 0, 0);
    Math::Matrix Frame::rotationMatrix;
    Math::Matrix Frame::rotationInverseMatrix;

    Math::Matrix Frame::worldviewMatrix;
    Math::Matrix Frame::worldviewInverseMatrix;

    Math::Matrix Frame::totalTransformationMatrix;

    Frustum Frame::WindowFrustum;

    void Frame::ConstructMatricies()
    {
	    Math::Vector position, rotation;

	    position=player->GetEyePosition();
	    rotation=player->GetCompositeRotation();

        Math::Matrix m = Util::Matrix::Translate(-position);
	    Math::Matrix m2 = Util::Matrix::RotateZ(-rotation.y);
        Math::Matrix m3 = Util::Matrix::RotateX(-rotation.x);
        Math::Matrix m4 = Util::Matrix::RotateY(rotation.z);

        Math::Matrix mi = Util::Matrix::Translate(position);
        Math::Matrix mi2 = Util::Matrix::RotateZ(rotation.y);
        Math::Matrix mi3 = Util::Matrix::RotateX(rotation.x);
        Math::Matrix mi4 = Util::Matrix::RotateY(-rotation.z);

	    worldviewMatrix=m4*m3*m2*m;
	    worldviewInverseMatrix=mi*mi2*mi3*mi4;
	    totalTransformationMatrix=perspectiveMatrix*worldviewMatrix;
	    rotationMatrix=m4*m3*m2;
	    rotationInverseMatrix=mi2*mi3*mi4;
    }

    void Frame::Render(float time)
    {
	    Math::Vector position, rotation;
        
	    position=player->GetEyePosition();
	    rotation=player->GetCompositeRotation();

	    glMatrixMode(GL_MODELVIEW);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glClear(GL_DEPTH_BUFFER_BIT);
	    glLoadIdentity();
        GLfloat m[16] = {1, 0, 0, 0, 
                         0, 0, -1, 0,
                         0, 1, 0, 0,
                         0, 0, 0, 1};
        glMultMatrixf(m);
    	
        glRotatef(-rotation.z,0,-1,0);
        glRotatef(-rotation.x,1,0,0);
        glRotatef(-rotation.y,0,0,1);
    	
	    glTranslatef(-position.x, -position.y, -position.z);
	    ConstructMatricies();

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

        OpenGl::SwapBuffers();

	    globalFlag++;
    }
    
    void Frame::SetPovModel(Model *model)
    {
        povModel = model;
    }

    Model *Frame::PovModel()
    {
        return povModel;
    }

    void Frame::PlayPovKey(Jk::Key *key, int flags)
    {
        povKeyTrack = Jk::Key::Track(key, 0, flags);
    }

    Jk::Key::Track *Frame::PovKey()
    {
        return &povKeyTrack;
    }
}