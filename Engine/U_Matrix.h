#ifndef U_MATRIX_H
#define U_MATRIX_H

#include "M_Matrix.h"

class U_Matrix
{
public:

    static M_Matrix Identity();
    static M_Matrix Scale(float scale);
    static M_Matrix Transform(float x, float y, float z);
    static M_Matrix RotateX(float angle);
    static M_Matrix RotateY(float angle);
    static M_Matrix RotateZ(float angle);
    
};
#endif