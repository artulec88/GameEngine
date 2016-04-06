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

const float width = 0.5;
const float edgeTransitionDistance = 0.1;
const float borderWidth = 0.4;
const float borderEdgeTransitionDistance = 0.05;
const vec2 offset = vec2(0.0, 0.0);
const vec3 outlineColor = vec3(0.2, 0.2, 0.2);

DeclareFragOutput(0, vec4);
void main()
{
	float distance = 1.0 - texture(fontAtlas, texCoord0).a;
	float alpha = 1.0 - smoothstep(width, width + edgeTransitionDistance, distance);
	
	float distance2 = 1.0 - texture(fontAtlas, texCoord0 + offset).a;
	float outlineAlpha = 1.0 - smoothstep(borderWidth, borderWidth + borderEdgeTransitionDistance, distance2);
	
	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColor = mix(outlineColor, textColor, alpha / overallAlpha);
	
	SetFragOutput(0, vec4(overallColor, overallAlpha));
}
#endif
