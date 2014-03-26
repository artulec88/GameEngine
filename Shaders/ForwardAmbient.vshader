#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;
uniform mat4 MVP;

out vec4 fragmentColor;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
	gl_Position = MVP * v;

	// The color of each vertex will be interpolated
	// to produce the color for each fragment
	fragmentColor = vec4(vertexColor, 1.0);
}