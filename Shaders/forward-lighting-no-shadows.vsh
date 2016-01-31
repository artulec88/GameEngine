attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;
//attribute vec3 biTangent;
out float gl_ClipDistance[1];

uniform mat4 T_model;
uniform mat4 T_MVP;

uniform vec4 R_clipPlane;

void setClippingPlanes(vec4 worldPosition, vec4 plane)
{
	gl_ClipDistance[0] = dot(worldPosition, plane);
}

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	vec4 worldPosition = T_model * vec4(position, 1.0);
    worldPos0 = worldPosition.xyz;
	
	setClippingPlanes(worldPosition, R_clipPlane);
	//setClippingPlanes(worldPosition, vec4(0, -1, 0, 4));
    
    vec3 n = normalize((T_model * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((T_model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t, biTangent, n);
}