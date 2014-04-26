#version 330 core
#include "Sampling.glh"

in vec2 texCoord0;
in vec3 worldPos0;
in mat3 tbnMatrix;

uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D displacementMap;
uniform float displacementScale;
uniform float displacementBias;

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	
	gl_FragColor = texture2D(diffuse, texCoords) * vec4(R_ambientIntensity, 1.0);
}