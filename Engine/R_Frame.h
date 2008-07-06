#ifndef R_FRAME_H
#define R_FRAME_H

#include "M_Matrix.h"
#include "M_Vector.h"

#define FOV 71

struct R_Frustum {
	double x0, x1;
	double y0, y1;

	double x00d, x10d;
	double x01d, x11d;

	M_Vector *planeNormals;
	int numPlanes;
};

extern int ScreenX, ScreenY;

extern float SX, SY;

extern M_Matrix perspectiveMatrix;
extern M_Matrix rotationMatrix;
extern M_Matrix rotationInverseMatrix;

extern M_Matrix worldviewMatrix;
extern M_Matrix worldviewInverseMatrix;

extern M_Matrix coordConversionMatrix;
extern M_Matrix coordConversionInverseMatrix;

extern M_Matrix totalTransformationMatrix;

extern int globalFlag;

extern R_Frustum R_WindowFrustum;

void R_Frame_Render();

void R_Frame_SetupClipPlanes( R_Frustum frustum );
void R_Frame_DisableClipPlanes();
#endif