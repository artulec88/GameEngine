#ifndef __GAME_TEXTURE_IDS_H__
#define __GAME_TEXTURE_IDS_H__

#include "Rendering/TextureIDs.h"

namespace game
{
	namespace texture_ids
	{
		enum TextureId
		{
			TERRAIN_DIFFUSE = rendering::texture_ids::COUNT,
			TERRAIN_DIFFUSE_2,
			TERRAIN_DIFFUSE_3,
			TERRAIN_DIFFUSE_4,
			TERRAIN_NORMAL_MAP,
			TERRAIN_DISPLACEMENT_MAP,
			TERRAIN_BLEND_MAP,
			BRICKS,
			BRICKS_NORMAL_MAP,
			BRICKS_DISPLACEMENT_MAP,
			TREE,
			BILLBOARD_TREE_1,
			BILLBOARD_TREE_2,
			BILLBOARD_TREE_3,
			BOULDER,
			BOULDER_NORMAL_MAP,
			CHESSBOARD,
			PLAYER,
			PLAYER_NORMAL_MAP,
			DEFAULT_NORMAL_MAP,
			SKYBOX_DAY,
			SKYBOX_NIGHT,
			PARTICLE_WATER,
			PARTICLE_FIRE,
			PARTICLE_SMOKE
		}; /* end enum TextureId */
	} /* end namespace texture_ids */
} /* end namespace game */

#endif /* __GAME_TEXTURE_IDS_H__ */