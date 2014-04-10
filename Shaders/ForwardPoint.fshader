#version 330 core
#include "Lighting.glh"

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuse;
uniform PointLight R_pointLight;

void main()
{
	fragColor = texture2D(diffuse, texCoord0.xy) *
		CalcPointLight(R_pointLight, normalize(normal0), worldPos0);
}
