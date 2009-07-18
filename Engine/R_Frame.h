#ifndef R_FRAME_H
#define R_FRAME_H

#include "M_Matrix.h"
#include "M_Vector.h"

#define FOV 71

namespace Render
{
    struct Frustum {
	    double x0, x1;
	    double y0, y1;

	    double x00d, x10d;
	    double x01d, x11d;

        Math::Vector *planeNormals;
	    int numPlanes;
    };

    extern int ScreenX, ScreenY;

    extern float SX, SY;

    extern Math::Matrix perspectiveMatrix;
    extern Math::Matrix rotationMatrix;
    extern Math::Matrix rotationInverseMatrix;

    extern Math::Matrix worldviewMatrix;
    extern Math::Matrix worldviewInverseMatrix;

    extern Math::Matrix coordConversionMatrix;
    extern Math::Matrix coordConversionInverseMatrix;

    extern Math::Matrix totalTransformationMatrix;

    extern Frustum WindowFrustum;

    void Frame_Render(float time);
}

extern int globalFlag;
#endif