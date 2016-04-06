#include "common.glh"

#if defined(VS_BUILD)
attribute vec2 position;

uniform mat4 T_MVP;

void main()
{
    gl_Position = T_MVP * vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, vec4(1.0));
}
#endif