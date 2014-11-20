#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

uniform mat4 transform;
uniform mat4 projectedTransform;

void main()
{
	gl_Position = projectedTransform * vec4(vertexPosition_modelspace, 1.0);
	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal, 0.0)).xyz; // this is swizzling :)
	worldPos0 = (transform * vec4(vertexPosition_modelspace, 1.0)).xyz;
}