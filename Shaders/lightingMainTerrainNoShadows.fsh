#include "parallaxMapping.glh"

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;
//uniform bool hasMultipleTextures;

DeclareFragOutput(0, vec4);
void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
	vec4 lightingAmt = CalcLightingEffect(normal, worldPos0);
	
	const float minimum = -2.8; // TODO: Get minimum and maximum height from the terrain mesh
	const float maximum = 1.8;
	const float limit = 0.9;
	vec4 texColor;
	
	if (worldPos0.y < limit)
	{
		texColor = mix(texture2D(diffuse, texCoords), texture2D(diffuse2, texCoords), (worldPos0.y - minimum) / (limit - minimum));
	}
	else
	{
		texColor = texture2D(diffuse2, texCoords);
	}
	SetFragOutput(0, texColor * lightingAmt);
	
	// vec4 diffuseTexColor = texture2D(diffuse, texCoords);
	// if (hasMultipleTextures)
	// {
		// vec4 diffuse2TexColor = texture2D(diffuse2, texCoords);
		// vec4 texColor = mix(diffuseTexColor, diffuse2TexColor, diffuse2TexColor.r + diffuse2TexColor.g + diffuse2TexColor.b);
		// SetFragOutput(0, texColor * lightingAmt);
	// }
	// else
	// {
		// SetFragOutput(0, diffuseTexColor * lightingAmt);
	// }
	
	// vec4 diffuseTexColor = texture2D(diffuse, texCoords);
	// SetFragOutput(0, diffuseTexColor * lightingAmt);
}