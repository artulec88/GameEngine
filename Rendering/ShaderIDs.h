#ifndef __RENDERING_SHADER_IDS_H__
#define __RENDERING_SHADER_IDS_H__

namespace rendering
{
	namespace shader_ids
	{
		enum ShaderId
		{
			INVALID = 0,
			AMBIENT,
			AMBIENT_TERRAIN,
			AMBIENT_FOG_LINEAR_PLANE_BASED,
			AMBIENT_FOG_EXPONENTIAL_PLANE_BASED,
			AMBIENT_FOG_LINEAR_RANGE_BASED,
			AMBIENT_FOG_EXPONENTIAL_RANGE_BASED,
			AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED,
			AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED,
			AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED,
			AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED,
			SHADOW_MAP,
			SHADOW_MAP_CUBE,
			FILTER_NULL,
			FILTER_GAUSSIAN_BLUR,
			FILTER_FXAA,
			SKYBOX,
			SKYBOX_PROCEDURAL,
			TEXT_SIMPLE,
			TEXT,
			WATER,
			WATER_NO_DIRECTIONAL_LIGHT,
			BILLBOARD,
			PARTICLES,
			PARTICLES_COLORS, // For particles that have colors attributes.
			PARTICLES_UNIFORM_COLORS, // For particles that all use the same uniform color.
			GUI,
			DIRECTIONAL_LIGHT,
			DIRECTIONAL_LIGHT_TERRAIN,
			DIRECTIONAL_LIGHT_NO_SHADOWS,
			DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS,
			POINT_LIGHT,
			POINT_LIGHT_TERRAIN,
			POINT_LIGHT_NO_SHADOWS,
			POINT_LIGHT_TERRAIN_NO_SHADOWS,
			SPOT_LIGHT,
			SPOT_LIGHT_TERRAIN,
			SPOT_LIGHT_NO_SHADOWS,
			SPOT_LIGHT_TERRAIN_NO_SHADOWS,
			// TODO: Add more shader types
			COUNT  // This value must always stay as the last one in the enum. It only represents the number of available basic shader IDs.
		}; /* end enum ShaderId */
	} /* end namespace shader_ids */
} /* end namespace rendering */

#endif /* __RENDERING_SHADER_IDS_H__ */