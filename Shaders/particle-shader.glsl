#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec2 position;

uniform mat4 T_MVP;

void main()
{
	texCoord0 = position + vec2(0.5, 0.5);
	texCoord0.y = 1.0 - texCoord0.y;
    gl_Position = T_MVP * vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

uniform sampler2D particleTexture;

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, texture(particleTexture, texCoord0));
}
#endif