#include "R_Sprite.h"

#include <gl/gl.h>

void R_Sprite::Draw()
{
	texture->Select(0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	glTexCoord2f(0, 0);
	glVertex3f(-width, -height, 0);

	glTexCoord2f(0, 1);
	glVertex3f(-width, height, 0);

	glTexCoord2f(1, 1);
	glVertex3f(width, height, 0);

	glTexCoord2f(1, 0);
	glVertex3f(width, -height, 0);
	glEnd();
}