#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec2 vVertex; // in clipping space
attribute vec2 vTexCoord;

uniform vec2 translation;

void main()
{
	gl_Position =  vec4(vVertex.x + translation.x * 2.0f, vVertex.y - translation.y * 2.0, 0.0, 1.0);
	texCoord0 = vTexCoord;
}

#elif defined(FS_BUILD)
uniform vec3 textColor;
uniform sampler2D fontAtlas;

// Watch https://www.youtube.com/watch?v=d8cfgcJR9Tk&index=33&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP
uniform vec2 offset;
uniform vec3 outlineColor;
uniform float characterWidth;
uniform float characterEdgeTransitionDistance;
uniform float borderWidth;
uniform float borderEdgeTransitionDistance;

DeclareFragOutput(0, vec4);
void main()
{
	float distance = 1.0 - texture(fontAtlas, texCoord0).a;
	float alpha = 1.0 - smoothstep(characterWidth, characterWidth + characterEdgeTransitionDistance, distance);
	
	float distance2 = 1.0 - texture(fontAtlas, texCoord0 + offset).a;
	float outlineAlpha = 1.0 - smoothstep(borderWidth, borderWidth + borderEdgeTransitionDistance, distance2);
	
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColor = mix(outlineColor, textColor, alpha / (overallAlpha + 0.000001));
	
	SetFragOutput(0, vec4(overallColor, overallAlpha));
}
#endif
