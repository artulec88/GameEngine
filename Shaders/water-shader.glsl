#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 T_MVP;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
    //texCoord0 = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);
	texCoord0 = texCoord;
}
#elif defined(FS_BUILD)

uniform sampler2D R_waterReflectionTexture;
//uniform sampler2D R_waterRefractionTexture;

DeclareFragOutput(0, vec4);
void main()
{
	vec4 reflectionColor = texture2D(R_waterReflectionTexture, texCoord0);
	//vec4 refractionColor = texture2D(R_waterRefractionTexture, texCoord0);
	//SetFragOutput(0, mix(reflectionColor, refractionColor, 0.5));
	
	SetFragOutput(0, reflectionColor);
}

#endif
