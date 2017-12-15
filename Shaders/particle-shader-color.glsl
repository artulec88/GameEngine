#include "common.glh"

varying vec4 vColor;

#if defined(VS_BUILD)
layout(location = 0) attribute vec2 position;
layout(location = 1) attribute mat4 mvp; // instanced attribute
layout(location = 5) attribute vec4 color; // instanced attribute

void main()
{
	vColor = color;
    gl_Position = mvp * vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, vColor);
}
#endif