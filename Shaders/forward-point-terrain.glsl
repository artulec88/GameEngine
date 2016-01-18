#include "common.glh"
varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "tiling-terrain.glh"
// forwardLighting is not OK for the point light due to the fact that shadows are calculated in a different way than for either directional or spot lights.
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;
//attribute vec3 biTangent;

uniform mat4 T_model;
uniform mat4 T_MVP;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
    texCoord0 = texCoord; 
    worldPos0 = (T_model * vec4(position, 1.0)).xyz;
    
    vec3 n = normalize((T_model * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((T_model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t, biTangent, n);
}
#elif defined(FS_BUILD)

#include "parallaxMapping.glh"
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

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;
layout(binding=5) uniform samplerCube R_cubeShadowMap;
//uniform bool hasMultipleTextures;

bool InRange(float val)
{
	return val >= 0.0 && val <= 1.0;
}

float CalcShadowAmount(samplerCube cubeShadowMap, vec3 lightDirection)
{
	float sampledDistance = texture(cubeShadowMap, lightDirection).r;
	float distance = length(lightDirection);
	if (distance < sampledDistance + 0.00001)
	{
		return 1.0; // inside the light
	}
	else
	{
		return 0.0; // inside the shadow
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
	vec3 lightDirection = worldPos0 - R_pointLight.position;
	vec4 lightingAmt = CalcLightingEffect(normal, worldPos0) * CalcShadowAmount(R_cubeShadowMap, lightDirection);
	
	const float minimum = -2.8; // TODO: Get minimum and maximum height from the terrain mesh
	const float maximum = 1.8;
	const float limit = 0.9;
	vec4 texColor = vec4(0.0);
	
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
#endif
