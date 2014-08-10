#version 330 core
#include "Lighting.fshader"

uniform SpotLight R_spotLight;

vec4 CalcLightingEffect(vec3 normal, vec3 worldPos)
{
	return CalcSpotLight(R_spotLight, normal, worldPos);
}

#include "LightingMain.fshader"