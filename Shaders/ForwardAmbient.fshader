#version 330 core

in vec2 texCoord0;

uniform vec3 R_ambientIntensity;
uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture2D(diffuse, texCoord0.xy) * vec4(R_ambientIntensity, 1.0);
}