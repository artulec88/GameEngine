#version 330 core

in vec4 color;
out vec4 fragColor;

void main()
{
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0); // the whole figure will be red
	fragColor = color;
}