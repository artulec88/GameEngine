#version 330 core
#include "Lighting.fshader"

uniform PointLight R_pointLight;

vec4 CalcLightingEffect(vec3 normal, vec3 worldPos)
{
	return CalcPointLight(R_pointLight, normal, worldPos);
}

#include "LightingMain.fshader"