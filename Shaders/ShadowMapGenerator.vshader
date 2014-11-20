#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 T_MVP;

void main()
{
	gl_Position = T_MVP * vec4(vertexPosition_modelspace, 1.0);
}
