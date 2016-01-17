#include "common.glh"

varying vec4 clipSpacePosition;
varying vec3 worldPos0;
varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_MVP;
uniform mat4 T_model;

const float TILING = 4.0;

void main()
{
	worldPos0 = (T_model * vec4(position, 1.0)).xyz;
	clipSpacePosition = T_MVP * vec4(position, 1.0);
    gl_Position = clipSpacePosition;
	texCoord0 = vec2(clipSpacePosition.x / 2.0 + 0.5, clipSpacePosition.y / 2.0 + 0.5) * TILING;
}
#elif defined(FS_BUILD)
#include "lighting.glh"

uniform float R_waterMoveFactor;
uniform vec3 C_eyePos;
uniform sampler2D R_waterReflectionTexture;
uniform sampler2D R_waterRefractionTexture;
uniform sampler2D R_waterDepthMap;
uniform sampler2D R_waterDUDVMap;
uniform sampler2D R_waterNormalMap;

uniform DirectionalLight R_directionalLight;

const float WAVE_STRENGTH = 0.04;
const float SHINE_DAMPER = 20.0;
const float REFLECTIVITY = 0.5;

DeclareFragOutput(0, vec4);
void main()
{
	// Projective texture mapping (watch https://www.youtube.com/watch?v=GADTasvDOX4)
	vec2 normalizedDeviceCoordinates = (clipSpacePosition.xy / clipSpacePosition.w) / 2.0 + 0.5;
	
	// Because it is a reflection we need to invert the Y component to make it upside down (it's reflection after all).
	vec2 reflectionTexCoord = vec2(normalizedDeviceCoordinates.x, -normalizedDeviceCoordinates.y);
	vec2 refractionTexCoord = vec2(normalizedDeviceCoordinates.x, normalizedDeviceCoordinates.y);
	
	// Watch: https://www.youtube.com/watch?v=qgDPSnZPGMA&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=8
	// We don't want the depth map to be distorted, so we sample the water depth map before distortion is performed.
	const float near = 0.1; // TODO: load them as uniforms. Must be equal to near plane for the camera.
	const float far = 1000.0; // TODO: load them as uniforms. Must be equal to far plane for the camera.
	float depth = texture2D(R_waterDepthMap, refractionTexCoord).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near)); // the distance from the camera to the object / terrain under the water.
	float waterSurfaceDistance = 2.0 * near * far / (far + near - (2.0 * gl_FragCoord.z - 1.0) * (far - near)); // the distance from the camera to the water surface.
	float waterDepth = floorDistance - waterSurfaceDistance;
	
	vec2 distortedTexCoords = texture(R_waterDUDVMap, vec2(texCoord0.x + R_waterMoveFactor, texCoord0.y)).rg * 0.1;
	distortedTexCoords = texCoord0 + vec2(distortedTexCoords.x, distortedTexCoords.y + R_waterMoveFactor);
	vec2 totalDistortion = (texture(R_waterDUDVMap, distortedTexCoords).rg * 2.0 - 1.0) * WAVE_STRENGTH * clamp(waterDepth / 40.0, 0.0, 1.0); // https://www.youtube.com/watch?v=qgDPSnZPGMA&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=8;
	
	reflectionTexCoord += totalDistortion;
	refractionTexCoord += totalDistortion;
	
	// Because of projection texturing the reflection and refraction texture coordinates at the bottom of the screen
	// are going to have a Y value of almost zero and because the distortion adds an offset to these values they sometimes may go below zero.
	// This causes the textures to go off the bottom of the texture and then wrap back around to the top of the texture,
	// because of the way OpenGL tiles them. To fix this glitch we need to clamp reflection and refraction texture
	// coordinates.
	reflectionTexCoord.x = clamp(reflectionTexCoord.x, 0.001, 0.999);
	reflectionTexCoord.y = clamp(reflectionTexCoord.y, -0.999, -0.001);
	refractionTexCoord = clamp(refractionTexCoord, 0.001, 0.999);
	
	vec4 reflectionColor = texture2D(R_waterReflectionTexture, reflectionTexCoord);
	vec4 refractionColor = texture2D(R_waterRefractionTexture, refractionTexCoord);
	
	vec4 normalMapColor = texture2D(R_waterNormalMap, distortedTexCoords);
	vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0 /* to make the normals a little bit more vertical */, normalMapColor.g * 2.0 - 1.0));
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	float refractiveFactor = clamp(pow(dot(directionToEye, normal), 2.0 /* the bigger the more reflective water will be */), 0.0, 1.0);
	
	vec3 reflectedLight = reflect(-R_directionalLight.direction, normal);
	float specular = pow(max(dot(directionToEye, reflectedLight), 0.0), SHINE_DAMPER);
	vec3 specularHighlights = R_directionalLight.base.color.xyz * specular * REFLECTIVITY * clamp(waterDepth / 2.0, 0.0, 1.0); // https://www.youtube.com/watch?v=qgDPSnZPGMA&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=8;
	
	vec4 outColor = mix(reflectionColor, refractionColor, refractiveFactor);
	outColor = mix(outColor, vec4(0.0, 0.3, 0.5, 1.0), 0.08) + vec4(specularHighlights, 0.0);
	outColor.a = clamp(waterDepth / 2.0, 0.0, 1.0); // https://www.youtube.com/watch?v=qgDPSnZPGMA&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh&index=8
	SetFragOutput(0, outColor);
}

#endif
