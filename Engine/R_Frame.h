#ifndef R_FRAME_H
#define R_FRAME_H

#include "M_Matrix.h"
#include "M_Vector.h"

#include "JK_Key.h"

#define FOV 71

namespace Render
{
    class Model;

    struct Frustum {
	    double x0, x1;
	    double y0, y1;

	    double x00d, x10d;
	    double x01d, x11d;

        Math::Vector *planeNormals;
	    int numPlanes;
    };

    class Frame
    {
    public:
        static int ScreenX, ScreenY;

        static float SX, SY;

        static Math::Matrix perspectiveMatrix;
        static Math::Matrix rotationMatrix;
        static Math::Matrix rotationInverseMatrix;

        static Math::Matrix worldviewMatrix;
        static Math::Matrix worldviewInverseMatrix;

        static Math::Matrix coordConversionMatrix;
        static Math::Matrix coordConversionInverseMatrix;

        static Math::Matrix totalTransformationMatrix;

        static Frustum WindowFrustum;

        static void Render(float time);

        static void SetPovModel(Model *model);
        static Model *PovModel();

        static void PlayPovKey(Jk::Key *key, int flags);
        static Jk::Key::Track *PovKey();

    private:
        static void ConstructMatricies();
    };
}

extern int globalFlag;
#endif