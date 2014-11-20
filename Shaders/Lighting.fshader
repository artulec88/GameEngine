in vec2 texCoord0;
in vec4 shadowMapCoord0;
in mat3 tbnMatrix;
in vec3 worldPos0;

out vec4 fragColor;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;
uniform float displacementScale;
uniform float displacementBias;

uniform sampler2D R_shadowMap;
//uniform float R_shadowBias; // used in basic, linear, PCF shadow mapping (not used in variance shadow mapping)
//uniform vec3 R_shadowTexelSize; // used in basic, linear, PCF shadow mapping (not used in variance shadow mapping)

#include "Lighting.glh"
