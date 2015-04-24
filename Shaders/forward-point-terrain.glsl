#include "common.glh"
#include "forwardlighting.glh"

#if defined(VS_BUILD)
#include "forwardlighting.vsh"
#elif defined(FS_BUILD)

#include "lighting.glh"

uniform vec3 C_eyePos;
uniform float specularIntensity;
uniform float specularPower;

uniform PointLight R_pointLight;

/**
 * Possible optimization technique- the surface that aren't oriented to light are in shadow.
 * See http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=35. First tests didn't prove that.
 */
// float CalculateDotBetweenLightDirectionAndNormal(vec3 normal)
// {
	// return 1.0; // Point light does not have a direction so it is impossible to calculate the dot product
// }

vec4 CalcLightingEffect(vec3 normal, vec3 worldPos)
{
	return CalcPointLight(R_pointLight, normal, worldPos,
	                      specularIntensity, specularPower, C_eyePos);
}

#include "lightingMainTerrain.fsh"
#endif
