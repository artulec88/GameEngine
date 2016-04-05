#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec2 vVertex; // in clipping space
attribute vec2 vTexCoord;

uniform vec3 translation;

void main()
{
	gl_Position =  vec4(vVertex.x + translation.x * 2.0f, vVertex.y - translation.y * 2.0, 0.0, 1.0);
	texCoord0 = vTexCoord;
}

#elif defined(FS_BUILD)
uniform vec3 textColor;
uniform sampler2D fontAtlas;

DeclareFragOutput(0, vec4);
void main()
{
	vec4 textureColor = texture(fontAtlas, texCoord0);
	//if (textureColor.a > 0.999) { SetFragOutput(0, vec4(texCoord0, 0.0, 1.0)); }
	SetFragOutput(0, vec4(textColor, textureColor.a));
	//SetFragOutput(0, vec4(texCoord0, 0.0, 1.0));
	//else
	//{
		//SetFragOutput(0, vec4(1.0, 1.0, 1.0, 1.0));
		//SetFragOutput(0, textureColor);
	//}
}
#endif
