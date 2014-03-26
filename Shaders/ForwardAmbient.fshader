#version 330 core

// Interpolated values from the vertex shaders
in vec4 fragmentColor;
out vec4 color;

void main()
{
	// Output color = color specified in the vertex shader,
	// interpolated between all 3 surounding vertices
	color = fragmentColor;
}