#ifndef U_MATRIX_H
#define U_MATRIX_H

#include "M_Matrix.h"

namespace Util
{
    class Matrix
    {
    public:

        static Math::Matrix Identity();
        static Math::Matrix Scale(float scale);
        static Math::Matrix Translate(float x, float y, float z);
        static Math::Matrix Translate(Math::Vector offset);
        static Math::Matrix RotateX(float angle);
        static Math::Matrix RotateY(float angle);
        static Math::Matrix RotateZ(float angle);
        
    };
}
#endif