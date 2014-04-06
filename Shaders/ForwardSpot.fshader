#version 330 core
#include "Lighting.glh"

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuseSampler;
uniform SpotLight spotLight;

void main()
{
	fragColor = texture2D(diffuseSampler, texCoord0.xy) *
		CalcSpotLight(spotLight, normalize(normal0), worldPos0);
}
