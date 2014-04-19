#version 330 core
#include "Lighting.glh"

in vec2 texCoord0;
in mat3 tbnMatrix;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform PointLight R_pointLight;

void main()
{
	vec3 normal = (255.0 / 128.0 * texture2D(normalMap, texCoord0.xy).xyz - 1) * tbnMatrix; // 255/128 may be switched to just "2", but it may prove to be better
	fragColor = texture2D(diffuse, texCoord0.xy) *
		CalcPointLight(R_pointLight, normalize(normal), worldPos0);
}
