#include "U_Matrix.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Util
{
    Math::Matrix Matrix::Identity()
    {
        return Math::Matrix(1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);
    }

    Math::Matrix Matrix::Scale(float scale)
    {
        return Math::Matrix(scale, 0, 0, 0,
                        0, scale, 0, 0,
                        0, 0, scale, 0,
                        0, 0, 0,     1);
    }

    Math::Matrix Matrix::Translate(float x, float y, float z)
    {
        return Math::Matrix(1, 0, 0, x,
                        0, 1, 0, y,
                        0, 0, 1, z,
                        0, 0, 0, 1);
    }

    Math::Matrix Matrix::Translate(Math::Vector offset)
    {
        return Translate(offset.x, offset.y, offset.z);
    }

    Math::Matrix Matrix::RotateX(float angle)
    {
        float c = cos( angle * M_PI / 180 );
	    float s = sin( angle * M_PI / 180 );

        return Math::Matrix ( 1, 0,  0, 0,
			              0, c, -s, 0,
			              0, s,  c, 0,
			              0, 0,  0, 1);
    }

    Math::Matrix Matrix::RotateY(float angle)
    {
        float c = cos( angle * M_PI / 180 );
	    float s = sin( angle * M_PI / 180 );

        return Math::Matrix (  c, 0, s, 0,
                           0, 1, 0, 0,
			              -s, 0, c, 0,
 			               0, 0, 0, 1);
    }

    Math::Matrix Matrix::RotateZ(float angle)
    {
        float c = cos( angle * M_PI / 180 );
	    float s = sin( angle * M_PI / 180 );

        return Math::Matrix ( c, -s, 0, 0,
			              s,  c, 0, 0,
                          0,  0, 1, 0,
			              0, 0,  0, 1);
    }
}
