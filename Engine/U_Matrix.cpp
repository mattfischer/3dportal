#include "U_Matrix.h"

#include <math.h>

M_Matrix U_Matrix::Identity()
{
    return M_Matrix(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

M_Matrix U_Matrix::Scale(float scale)
{
    return M_Matrix(scale, 0, 0, 0,
                    0, scale, 0, 0,
                    0, 0, scale, 0,
                    0, 0, 0,     1);
}

M_Matrix U_Matrix::Transform(float x, float y, float z)
{
    return M_Matrix(1, 0, 0, x,
                    0, 1, 0, y,
                    0, 0, 1, z,
                    0, 0, 0, 1);
}

M_Matrix U_Matrix::RotateX(float angle)
{
    float c = cos( angle * 3.14 / 180 );
	float s = sin( angle * 3.14 / 180 );

    return M_Matrix ( 1, 0,  0, 0,
			          0, c, -s, 0,
			          0, s,  c, 0,
			          0, 0,  0, 1);
}

M_Matrix U_Matrix::RotateY(float angle)
{
    float c = cos( angle * 3.14 / 180 );
	float s = sin( angle * 3.14 / 180 );

    return M_Matrix (  c, 0, s, 0,
                       0, 1, 0, 0,
			          -s, 0, c, 0,
 			           0, 0, 0, 1);
}

M_Matrix U_Matrix::RotateZ(float angle)
{
    float c = cos( angle * 3.14 / 180 );
	float s = sin( angle * 3.14 / 180 );

    return M_Matrix ( c, -s, 0, 0,
			          s,  c, 0, 0,
                      0,  0, 1, 0,
			          0, 0,  0, 1);
}
