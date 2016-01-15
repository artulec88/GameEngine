#include "common.glh"

varying vec4 clipSpacePosition;

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_MVP;

void main()
{
	clipSpacePosition = T_MVP * vec4(position, 1.0);
    gl_Position = clipSpacePosition;
}
#elif defined(FS_BUILD)

uniform sampler2D R_waterReflectionTexture;
uniform sampler2D R_waterRefractionTexture;

DeclareFragOutput(0, vec4);
void main()
{
	// Projective texture mapping (watch https://www.youtube.com/watch?v=GADTasvDOX4)
	vec2 normalizedDeviceCoordinates = (clipSpacePosition.xy / clipSpacePosition.w) / 2.0 + 0.5;
	
	// Because it is a reflection we need to invert the Y component to make it upside down (it's reflection after all).
	vec2 reflectionTexCoord = vec2(normalizedDeviceCoordinates.x, -normalizedDeviceCoordinates.y);
	vec2 refractionTexCoord = vec2(normalizedDeviceCoordinates.x, normalizedDeviceCoordinates.y);
	
	vec4 reflectionColor = texture2D(R_waterReflectionTexture, reflectionTexCoord);
	vec4 refractionColor = texture2D(R_waterRefractionTexture, refractionTexCoord);
	SetFragOutput(0, mix(reflectionColor, refractionColor, 0.5));
	
	//SetFragOutput(0, reflectionColor);
}

#endif
