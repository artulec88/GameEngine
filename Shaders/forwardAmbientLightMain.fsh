uniform vec4 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
#if defined(FOG_ENABLED)
	float distance = CalcFogDistance(C_eyePos, worldPos0);
	float fogFactor = CalcFogFactor(R_ambientFogStart, R_ambientFogEnd, R_ambientFogDensityFactor, R_ambientFogGradient, distance);
#endif
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	//vec2 texCoords = texCoord0;
	
#if defined(FOG_ENABLED)
	SetFragOutput(0, mix(texture2D(diffuse, texCoords) * R_ambientIntensity, R_ambientFogColor, fogFactor));
#else
	SetFragOutput(0, texture2D(diffuse, texCoords) * R_ambientIntensity);
	//SetFragOutput(0, vec4(texCoords, 0.0, 1.0));
#endif
}