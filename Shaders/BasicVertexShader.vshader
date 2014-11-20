#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 color;

uniform mat4 transform;

void main()
{
	color = transform * vec4(clamp(vertexPosition_modelspace, 0.0, 1.0), 1.0);
	//gl_Position = vec4(vertexPosition_modelspace, 1.0);
	gl_Position = transform * vec4(vertexPosition_modelspace, 1.0);
}