#include "R_Sprite.h"

#include <gl/gl.h>

namespace Render
{
    void Sprite::Draw( int frame )
    {
	    if( frame >= texture->NumCels() ) return;

	    texture->Select(0, frame);
	    glDisable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);

	    glBegin(GL_POLYGON);
	    glColor3f(1, 1, 1);

	    glTexCoord2f(0, 0);
	    glVertex3f(-width / 2, 0, height / 2);

	    glTexCoord2f(1, 0);
	    glVertex3f(width / 2, 0, height / 2);

	    glTexCoord2f(1, 1);
	    glVertex3f(width / 2, 0, -height / 2);

	    glTexCoord2f(0, 1);
	    glVertex3f(-width / 2, 0, -height / 2);
	    glEnd();

	    glEnable(GL_DEPTH_TEST);
    }
}