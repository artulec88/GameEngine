#ifndef __GAME_MESH_IDS_H__
#define __GAME_MESH_IDS_H__

#include "Rendering\MeshIDs.h"

namespace Game
{
	namespace MeshIDs
	{
		enum MeshID
		{
			FIRST_MESH = Rendering::MeshIDs::COUNT,
			PLAYER,
			BILLBOARD,
			SKYBOX
		}; /* end enum MeshID */
	} /* end namespace MeshIDs */
} /* end namespace Game */

#endif /* __GAME_MESH_IDS_H__ */