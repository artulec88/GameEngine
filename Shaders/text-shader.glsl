#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec2 vVertex; // in clipping space
attribute vec2 vTexCoord;

uniform float screenWidth;
uniform float screenHeight;
//uniform mat4 MVP;

void main()
{
	gl_Position =  vec4(vVertex, 0, 1);
	texCoord0 = vTexCoord;
	
	//texCoord0 = vTexCoord;
    //gl_Position = MVP * vec4(vVertex, 0.0, 1.0);
}

#elif defined(FS_BUILD)
uniform vec4 textColor;
uniform sampler2D diffuse; // font texture

DeclareFragOutput(0, vec4);
void main()
{
	//vec4 vFragColor = texture(diffuse, texCoord0.st) * vec4(1.0, 0.0, 0.0, 1.0);
	vec4 vFragColor = texture(diffuse, texCoord0.st) * textColor;
	if (vFragColor.rgb == vec3(0.0, 0.0, 0.0))
	{
		//SetFragOutput(0, vec4(0.0, 1.0, 0.0, 1.0));
		discard;
	}
	SetFragOutput(0, vFragColor);
}
#endif
