#include "sampling.glh"

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;
//uniform bool hasMultipleTextures;

uniform sampler2D R_shadowMap;
uniform float R_shadowVarianceMin;
uniform float R_shadowLightBleedingReductionFactor;
uniform float R_dayNightMixFactor; // within range [0; 1] where 0 means total night and 1 means total day

bool InRange(float val)
{
	return val >= 0.0 && val <= 1.0;
}

float CalcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz/initialShadowMapCoords.w);
	
	if(InRange(shadowMapCoords.z) && InRange(shadowMapCoords.x) && InRange(shadowMapCoords.y))
	{
		return SampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z, R_shadowVarianceMin, R_shadowLightBleedingReductionFactor);
	}
	else
	{
		return R_dayNightMixFactor; // point is outside the range, so we render it in light
		// TODO: When we have in-game night we should instead return 0.0 here.
	}
}

DeclareFragOutput(0, vec4);
void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
    
    vec4 lightingAmt = CalcLightingEffect(normal, worldPos0) * CalcShadowAmount(R_shadowMap, shadowMapCoords0);
	
	const float minimum = -2.8; // TODO: Get minimum and maximum height from the terrain mesh
	const float maximum = 1.8;
	const float limit = 0.9;
	vec4 texColor = 0.0;
	
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
