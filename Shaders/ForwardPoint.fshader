#version 330 core
#include "Lighting.glh"

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuseSampler;
uniform PointLight pointLight;

void main()
{
	fragColor = texture2D(diffuseSampler, texCoord0.xy) *
		CalcPointLight(pointLight, normalize(normal0), worldPos0);
}
