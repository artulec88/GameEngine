#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

uniform mat4 T_model;
uniform mat4 T_MVP;

void main()
{
	gl_Position = vec4(vertexPosition_modelspace, 1.0) * T_MVP;
	texCoord0 = texCoord;
	normal0 = (vec4(normal, 0.0) * T_model).xyz; // this is swizzling :)
	worldPos0 = (vec4(vertexPosition_modelspace, 1.0) * T_model).xyz;
}
