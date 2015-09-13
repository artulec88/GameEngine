uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
	float distance = CalcFogDistance(C_eyePos, worldPos0, gl_FragCoord);
	float fogFactor = CalcFogFactor(R_ambientFogStart, R_ambientFogEnd, R_ambientFogDensityFactor, distance);
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	//vec2 texCoords = texCoord0;
	
	SetFragOutput(0, mix(texture2D(diffuse, texCoords), vec4(R_ambientFogColor, 1.0), fogFactor) * vec4(R_ambientIntensity, 1));
}