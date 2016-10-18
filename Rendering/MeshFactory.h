#ifndef __RENDERING_MESH_FACTORY_H__
#define __RENDERING_MESH_FACTORY_H__

#include "Rendering.h"
#include "Mesh.h"

#include "Utility\ILogger.h"

#include <map>

namespace Rendering
{
	/// <summary>
	/// <code>Mesh</code> factory.
	/// </summary>
	class MeshFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The mesh factory constructor.</summary>
		/// <param name="modelsDirectory"> The models directory where all meshes are stored. </param>
		RENDERING_API MeshFactory(const std::string& modelsDirectory);

		/// <summary>The destructor.</summary>
		RENDERING_API ~MeshFactory(void);

		/// <summary> Mesh factory copy constructor. </summary>
		MeshFactory(const MeshFactory& meshFactory) = delete;
		/// <summary> Mesh factory move constructor. </summary>
		MeshFactory(MeshFactory&& meshFactory) = delete;
		/// <summary> Mesh factory copy assignment operator. </summary>
		MeshFactory& operator=(MeshFactory& meshFactory) = delete;
		/// <summary> Mesh factory move assignment operator. </summary>
		MeshFactory& operator=(MeshFactory&& meshFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API const Mesh* CreateMesh(int meshID, const std::string& meshFileName);
		RENDERING_API inline const Mesh* GetMesh(int meshID) const
		{
			auto meshItr = m_meshType2MeshMap.find(meshID);
			if (meshItr == m_meshType2MeshMap.end())
			{
				auto instanceMeshItr = m_meshType2InstanceMeshMap.find(meshID);
				if (instanceMeshItr == m_meshType2InstanceMeshMap.end())
				{
					ERROR_LOG_RENDERING("No mesh has been created for the specified ID (", meshID, ").");
				}
				return &instanceMeshItr->second;
			}
			return &meshItr->second;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The system directory where all meshes are stored.
		/// </summary>
		const std::string& m_modelsDirectory;

		/// <summary>
		/// The map storing all meshes by their unique ID.
		/// </summary>
		std::map<int, Mesh> m_meshType2MeshMap;

		/// <summary>
		/// The map storing all instance meshes by their unique ID.
		/// </summary>
		std::map<int, InstanceMesh> m_meshType2InstanceMeshMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshFactory */

} /* end namespace Rendering */

#endif /* __RENDERING_MESH_FACTORY_H__ */