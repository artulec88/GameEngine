#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 color;

uniform float uniformFloat;
uniform mat4 transform;

void main()
{
	color = vec4(clamp(vertexPosition_modelspace, 0.0, uniformFloat), 1.0) * transform;
	//gl_Position = vec4(vertexPosition_modelspace, 1.0);
	gl_Position = vec4(vertexPosition_modelspace, 1.0) * transform;
}