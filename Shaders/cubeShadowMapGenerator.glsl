/**
 * http://http.developer.nvidia.com/GPUGems3/gpugems3_ch08.html
*/
#include "common.glh"

varying vec3 worldPos0;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 T_MVP;
uniform mat4 T_model;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
	worldPos0 = (T_model * vec4(position, 1.0)).xyz;
}
#elif defined(FS_BUILD)
uniform vec3 C_eyePos;

DeclareFragOutput(0, float);
void main()
{
	SetFragOutput(0, length(worldPos0 - C_eyePos));
}
#endif
