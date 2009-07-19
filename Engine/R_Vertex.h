#ifndef R_VERTEX_H
#define R_VERTEX_H

#include "M_Vector.h"

namespace Render
{
    struct TextureVertex {
	    float u;
	    float v;
    };

    struct Vertex {
	    Math::Vector position;
	    TextureVertex texture;
	    float intensity;
	    Math::Vector normal;
    };

    struct ModelVertex {
	    Math::Vector position;
	    TextureVertex texture;
	    Math::Vector normal;
    };
}

#endif
