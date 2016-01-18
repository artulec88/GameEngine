#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "tiling-terrain.glh"
#include "forwardAmbientLighting.vsh"
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "sampling.glh"

uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	//vec2 texCoords = texCoord0;

	// vec4 diffuseTexColor = texture2D(diffuse, texCoords);
	// vec4 diffuse2TexColor = texture2D(diffuse2, texCoords);
	// vec4 texColor = mix(diffuseTexColor, diffuse2TexColor, diffuse2TexColor.a);
	
	const float minimum = -2.8;
	const float maximum = 1.8;
	const float limit = 0.9;
	vec4 texColor = vec4(0.0);
	
	// This line gives the same results as the if..else clauses, but is slower (around 0.6 ms)
	// texColor = mix(texture2D(diffuse, texCoords), texture2D(diffuse2, texCoords), clamp((worldPos0.y - minimum) / (limit - minimum), 0.0, 1.0));
	if (worldPos0.y < limit)
	{
		texColor = mix(texture2D(diffuse, texCoords), texture2D(diffuse2, texCoords), (worldPos0.y - minimum) / (limit - minimum));
	}
	else
	{
		texColor = texture2D(diffuse2, texCoords);
	}
	
	SetFragOutput(0, texColor * vec4(R_ambientIntensity, 1));
}
#endif
