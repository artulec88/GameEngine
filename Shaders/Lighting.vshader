layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

out vec2 texCoord0;
out vec4 shadowMapCoord0;
out mat3 tbnMatrix;
out vec3 worldPos0;

uniform mat4 T_model;
uniform mat4 T_MVP;
uniform mat4 R_lightMatrix;

void main()
{
	gl_Position = vec4(vertexPosition_modelspace, 1.0) * T_MVP;
	texCoord0 = texCoord;
	shadowMapCoord0 = vec4(vertexPosition_modelspace, 1.0) * R_lightMatrix;
	worldPos0 = (vec4(vertexPosition_modelspace, 1.0) * T_model).xyz;
	
	vec3 n = normalize((vec4(normal, 0.0) * T_model).xyz); // this is swizzling :)
	vec3 t = normalize((vec4(tangent, 0.0) * T_model).xyz); // this is swizzling :)
	
	// Gramm-Schmidt orthogonalization
	t = normalize(t - dot(t, n) * n);
	
	vec3 bitangent = cross(t, n);
	
	tbnMatrix = mat3(t, bitangent, n);
}
