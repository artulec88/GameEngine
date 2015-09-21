attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;
//attribute vec3 biTangent;

uniform mat4 T_model;
uniform mat4 T_MVP;
uniform mat4 R_lightMatrix;

uniform vec3 R_clipPlaneNormal;
uniform float R_clipPlaneOriginDistance;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
    texCoord0 = texCoord; 
    shadowMapCoords0 = R_lightMatrix * vec4(position, 1.0);
	vec4 worldPosition = T_model * vec4(position, 1.0);
    worldPos0 = worldPosition.xyz;
	
	gl_ClipDistance[0] = dot(worldPosition, vec4(R_clipPlaneNormal, R_clipPlaneOriginDistance));
    
    vec3 n = normalize((T_model * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((T_model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t, biTangent, n);
}