#include "Sampling.glh"

float CalcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = initialShadowMapCoords.xyz / initialShadowMapCoords.w;
	//return SampleShadowMapPCF(shadowMap, shadowMapCoords.xy, shadowMapCoords.z - R_shadowBias, R_shadowTexelSize.xy); // PCF shadow mapping
	return SampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z); // Variance shadow mapping
}

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	
	vec3 normal = normalize(tbnMatrix * (255.0 / 128.0 * texture2D(normalMap, texCoords).xyz - 1)); // 255/128 may be switched to just "2", but it may prove to be better
	
	float shadowAmount = CalcShadowAmount(R_shadowMap, shadowMapCoord0);
	vec4 lightingAmount = CalcLightingEffect(normalize(normal), worldPos0) * shadowAmount;
	fragColor = texture2D(diffuse, texCoords) * lightingAmount;
	//fragColor = vec4(texCoords, 0.0, 0.0);
}
