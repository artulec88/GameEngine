#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;

out vec2 texCoord0;

uniform mat4 transform;

void main()
{
	gl_Position = vec4(vertexPosition_modelspace, 1.0) * transform;
	texCoord0 = texCoord;
}