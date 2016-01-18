#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "forwardAmbientLighting.vsh"
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "sampling.glh"

uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;

uniform float displacementScale; // displacement scale
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	//vec2 texCoords = texCoord0;
	
	SetFragOutput(0, texture2D(diffuse, texCoords) * vec4(R_ambientIntensity, 1));
}
#endif
