#include "parallaxMapping.glh"
#include "sampling.glh"

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

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
		return 1.0;
		//return R_dayNightMixFactor; // point is outside the range, so we render it in light
		// TODO: When we have in-game night we should instead return 0.0 here.
	}
}

DeclareFragOutput(0, vec4);
void main()
{
	/**
	 * The cosine value of the angle between light direction and the surface normal vector.
	 * This value may also be used in the shadow calculation for optimized shadows (see http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=35)
	 * TODO: If the cosine is required in both light and shadow calculation then maybe we should determine the cosine once and pass it to CalcLight
	 * and CalcShadowAmount functions.
	 */
	// if (CalculateDotBetweenLightDirectionAndNormal(normal) < 0) // optimization technique- See http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=35. First tests didn't give any optimization.
	// {
		// SetFragOutput(0, vec4(0, 0, 0, 1));
	// }
	
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
	vec4 lightingAmt = CalcLightingEffect(normal, worldPos0) * CalcShadowAmount(R_shadowMap, shadowMapCoords0);
	SetFragOutput(0, texture2D(diffuse, texCoords) * lightingAmt);
	
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
