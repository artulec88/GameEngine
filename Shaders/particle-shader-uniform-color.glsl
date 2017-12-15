#include "common.glh"

#if defined(VS_BUILD)
layout(location = 0) attribute vec2 position;
layout(location = 1) attribute mat4 mvp; // instanced attribute

void main()
{
    gl_Position = mvp * vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

uniform vec4 R_particleColor;

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, R_particleColor);
}
#endif