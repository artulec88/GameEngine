#version 330 core

in vec2 texCoord0;

uniform vec3 color;
uniform sampler2D sampler;

void main()
{
	vec4 textureColor = texture2D(sampler, texCoord0.xy);
	vec4 materialColor = vec4(color, 1.0);

	gl_FragColor = materialColor;
	
	if (textureColor != vec4(0.0, 0.0, 0.0, 0.0))
	{
		gl_FragColor *= textureColor;
	}
}