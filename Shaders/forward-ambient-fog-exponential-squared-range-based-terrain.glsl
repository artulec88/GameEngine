#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "tiling-terrain.glh"
#include "forwardAmbientLighting.vsh"
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "sampling.glh"
#include "forwardAmbientFogUniforms.glh"
#include "fogFallOffCalculationExponentialSquared.glh"
#include "fogDistanceCalculationRangeBased.glh"

#include "forwardAmbientLightMainTerrain.fsh"
#endif
