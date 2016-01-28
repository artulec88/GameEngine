#include "common.glh"
#include "forward-lighting.glh"

#if defined(VS_BUILD)
#include "tiling.glh"
#include "forward-lighting.vsh"
#elif defined(FS_BUILD)

#include "lighting.glh"

uniform vec3 C_eyePos;
uniform float specularIntensity;
uniform float specularPower;

uniform DirectionalLight R_directionalLight;

/**
 * Possible optimization technique- the surface that aren't oriented to light are in shadow.
 * See http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=35. First tests didn't prove that.
 */
// float CalculateDotBetweenLightDirectionAndNormal(vec3 normal)
// {
	// return dot(normal, R_directionalLight.direction);
// }

vec4 CalcLightingEffect(vec3 normal, vec3 worldPos)
{
	return CalcLight(R_directionalLight.base, -R_directionalLight.direction, normal, worldPos,
	                 specularIntensity, specularPower, C_eyePos);
}

#include "lightingMain.fsh"
#endif
