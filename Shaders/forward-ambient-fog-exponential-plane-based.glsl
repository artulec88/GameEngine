#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;

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
#include "sampling.glh"

uniform vec3 R_ambientFogColor;
uniform float R_ambientFogDensityFactor;
uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
	float distance = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = 1.0 - clamp(exp(-distance * R_ambientFogDensityFactor), 0.0, 1.0);
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	//vec2 texCoords = texCoord0;
	
	SetFragOutput(0, mix(texture2D(diffuse, texCoords), vec4(R_ambientFogColor, 1.0), fogFactor) * vec4(R_ambientIntensity, 1));
}
#endif
