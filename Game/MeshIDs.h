#ifndef __GAME_MESH_IDS_H__
#define __GAME_MESH_IDS_H__

#include "Rendering/MeshIDs.h"

namespace game
{
	namespace mesh_ids
	{
		enum MeshId
		{
			FIRST_MESH = rendering::mesh_ids::COUNT,
			PLAYER,
			BILLBOARD,
			SKYBOX
		}; /* end enum MeshId */
	} /* end namespace mesh_ids */
} /* end namespace game */

#endif /* __GAME_MESH_IDS_H__ */