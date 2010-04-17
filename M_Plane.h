#ifndef M_PLANE_H
#define M_PLANE_H

#include "M_Vector.h"

namespace Math
{
    struct Plane {
	    Vector normal;
	    Vector point;
    };
}
#endif