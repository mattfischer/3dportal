#ifndef R_VERTEX_H
#define R_VERTEX_H

#include "Global.h"

#include "M_Vector.h"

struct R_TextureVertex {
	float u;
	float v;
};

struct R_Vertex {
	Math::Vector position;
	R_TextureVertex texture;
	float intensity;
	Math::Vector normal;
};

struct R_ModelVertex {
	Math::Vector position;
	R_TextureVertex texture;
	Math::Vector normal;
};

#endif
