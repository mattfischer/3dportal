#ifndef M_PLANE_H
#define M_PLANE_H

#include "M_Vector.h"

struct M_Plane {
	M_Vector normal;
	M_Vector point;
};

#endif