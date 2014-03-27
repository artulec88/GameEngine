#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

out vec4 color;

// Values that stay constant for the whole mesh
// The fragment shader needs a "sampler2D" in order to know which texture to access
// (you can access several textures in the same shader)
uniform sampler2D myTextureSampler;

void main()
{
	// Output color = color of the texture at the specified UV
	color = vec4(texture(myTextureSampler, UV).rgb, 1.0);
}
