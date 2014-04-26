#version 330 core
#include "Lighting.glh"
#include "Sampling.glh"

in vec2 texCoord0;
in mat3 tbnMatrix;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;
uniform float displacementScale;
uniform float displacementBias;
uniform PointLight R_pointLight;

void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	
	vec3 normal = (255.0 / 128.0 * texture2D(normalMap, texCoords).xyz - 1) * tbnMatrix; // 255/128 may be switched to just "2", but it may prove to be better
	fragColor = texture2D(diffuse, texCoords) *
		CalcPointLight(R_pointLight, normalize(normal), worldPos0);
}
