#include "common.glh"

varying vec4 clipSpacePosition;
varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_MVP;

const float TILING = 6.0;

void main()
{
	clipSpacePosition = T_MVP * vec4(position, 1.0);
    gl_Position = clipSpacePosition;
	texCoord0 = vec2(clipSpacePosition.x / 2.0 + 0.5, clipSpacePosition.y / 2.0 + 0.5) * TILING;
}
#elif defined(FS_BUILD)

uniform float R_waterMoveFactor;
uniform sampler2D R_waterReflectionTexture;
uniform sampler2D R_waterRefractionTexture;
uniform sampler2D R_waterDUDVMap;

const float WAVE_STRENGTH = 0.02;

DeclareFragOutput(0, vec4);
void main()
{
	// Projective texture mapping (watch https://www.youtube.com/watch?v=GADTasvDOX4)
	vec2 normalizedDeviceCoordinates = (clipSpacePosition.xy / clipSpacePosition.w) / 2.0 + 0.5;
	
	// Because it is a reflection we need to invert the Y component to make it upside down (it's reflection after all).
	vec2 reflectionTexCoord = vec2(normalizedDeviceCoordinates.x, -normalizedDeviceCoordinates.y);
	vec2 refractionTexCoord = vec2(normalizedDeviceCoordinates.x, normalizedDeviceCoordinates.y);
	
	vec2 distortion1 = (texture2D(R_waterDUDVMap, vec2(texCoord0.x + R_waterMoveFactor, texCoord0.y)).rg * 2.0 - 1.0) * WAVE_STRENGTH;
	
	reflectionTexCoord += distortion1;
	refractionTexCoord += distortion1;
	
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
	SetFragOutput(0, mix(reflectionColor, refractionColor, 0.5));
	
	//SetFragOutput(0, reflectionColor);
}

#endif
