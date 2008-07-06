#ifndef R_VERTEX_H
#define R_VERTEX_H

#include "Global.h"

#include "M_Vector.h"

struct R_TextureVertex {
	float u;
	float v;
};

struct R_Vertex {
	M_Vector position;
	R_TextureVertex texture;
	float intensity;
	M_Vector normal;
};

struct R_ModelVertex {
	M_Vector position;
	R_TextureVertex texture;
	M_Vector normal;
};

#endif
