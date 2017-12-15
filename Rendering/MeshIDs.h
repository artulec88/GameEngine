#ifndef __RENDERING_MESH_IDS_H__
#define __RENDERING_MESH_IDS_H__

namespace rendering
{
	namespace mesh_ids
	{
		enum MeshId
		{
			INVALID,
			SIMPLE_PLANE,
			PARTICLE,
			PARTICLE_COLOR,
			PARTICLE_UNIFORM_COLOR,
			DEBUG,
			//TERRAIN,
			COUNT,
		}; /* end enum MeshId */
	} /* end namespace mesh_ids */
} /* end namespace rendering */

#endif /* __RENDERING_MESH_IDS_H__ */