#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "tiling.glh"
#include "forwardAmbientLighting.vsh"
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "sampling.glh"
#include "forwardAmbientFogUniforms.glh"
#include "fogFallOffCalculationExponential.glh"
#include "fogDistanceCalculationPlaneBased.glh"

#include "forwardAmbientLightMain.fsh"
#endif
