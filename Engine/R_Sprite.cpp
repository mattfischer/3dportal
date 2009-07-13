#include "R_Sprite.h"

#include <gl/gl.h>

void R_Sprite::Draw( int frame )
{
	if( frame >= texture->NumCels() ) return;

	texture->Select(0, frame);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glTexCoord2f(0, 0);
	glVertex3f(-width / 2, -height / 2, 0);

	glTexCoord2f(0, 1);
	glVertex3f(-width / 2, height / 2, 0);

	glTexCoord2f(1, 1);
	glVertex3f(width / 2, height / 2, 0);

	glTexCoord2f(1, 0);
	glVertex3f(width, -height, 0);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}