#ifndef __RENDERING_MESH_FACTORY_H__
#define __RENDERING_MESH_FACTORY_H__

#include "Rendering.h"
#include "Mesh.h"
#include "Image.h"

#include "Math/Surface.h"

#include "Utility/ILogger.h"

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
		/// <param name="texturesDirectory"> The textures directory where all textures are stored. </param>
		RENDERING_API MeshFactory(const std::string& modelsDirectory, const std::string& texturesDirectory);

		/// <summary>The destructor.</summary>
		RENDERING_API ~MeshFactory(void);

		/// <summary> Mesh factory copy constructor. </summary>
		/// <param name="meshFactory"> The mesh factory to copy construct from. </param>
		MeshFactory(const MeshFactory& meshFactory) = delete;

		/// <summary> Mesh factory move constructor. </summary>
		/// <param name="meshFactory"> The mesh factory to move construct from. </param>
		MeshFactory(MeshFactory&& meshFactory) = delete;

		/// <summary> Mesh factory copy assignment operator. </summary>
		/// <param name="meshFactory"> The mesh factory to copy assign from. </param>
		/// <returns> The reference of the newly copy-assigned mesh factory. </returns>
		MeshFactory& operator=(MeshFactory& meshFactory) = delete;

		/// <summary> Mesh factory move assignment operator. </summary>
		/// <param name="meshFactory"> The mesh factory to move assign from. </param>
		/// <returns> The reference of the newly move-assigned mesh factory. </returns>
		MeshFactory& operator=(MeshFactory&& meshFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Creates a mesh based on the model definition in given file (<paramref name="meshFileName"/>).
		/// </summary>
		/// <param name="meshID"> The ID of the mesh. This ID might later be used to access that particular mesh. </param>
		/// <param name="meshFileName">
		/// The system path where the model is stored. The model will be used to create mesh positions, texture coordinates, normals etc.
		/// </param>
		/// <returns> The mesh loaded from the specified model. </returns>
		RENDERING_API const Mesh* CreateMesh(int meshID, const std::string& meshFileName);

		/// <summary>
		/// Creates a uniform mesh from the specified surface given in <paramref name="surface"/>.
		/// The result is a procedurally generated mesh with vertices uniformly laid out on the XZ plane.
		/// The final mesh will consist of exactly the same number of vertices as the number of heights values stored by the surface.
		/// </summary>
		/// <param name="meshID"> The ID of the mesh. This ID might later be used to access that particular mesh. </param>
		/// <param name="surface">
		/// The surface. It will be used to create mesh positions, texture coordinates, normals etc.
		/// </param>
		/// <returns> The procedurally generated mesh from the specified surface. </returns>
		RENDERING_API const Mesh* CreateMeshFromSurface(int meshID, const math::Surface& surface);

		//RENDERING_API const TerrainMesh* CreateTerrainMesh(int meshID, int gridX, int gridZ, const math::HeightsGenerator& heightsGenerator, int vertexCount);
		//RENDERING_API const Mesh* CreateBillboardMesh(int meshID, const std::string& meshFileName);
		//RENDERING_API const Mesh* CreateInstanceMesh(int meshID, const std::string& meshFileName);
		//RENDERING_API const Mesh* CreateTerrainMesh(int meshID, const std::string& meshFileName);
		RENDERING_API inline const Mesh* GetMesh(int meshID) const
		{
			auto meshItr = m_meshType2MeshMap.find(meshID);
			CHECK_CONDITION_EXIT_RENDERING(meshItr != m_meshType2MeshMap.end(), Utility::Logging::ERR,
				"No mesh has been created for the specified ID (", meshID, ").");
			return meshItr->second.get();
		}
	private:
		int GetHeightMapIndex(int xIndex, int zIndex, int horizontalVerticesCount) const
		{
			return zIndex * horizontalVerticesCount + xIndex;
		}
		math::Real CalculateHeightAt(int x, int z, const Image& heightMapImage, math::Real heightMapMaxHeight) const;
		math::Real CalculateHeightAt(int x, int z, int heightMapWidth, const math::HeightsGenerator& heightsGenerator) const;
		math::Vector3D CalculateNormal(int x, int z, const math::Surface& surface) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The system directory where all meshes are stored.
		/// </summary>
		const std::string& m_modelsDirectory;

		/// <summary>
		/// The system directory where all textures are stored.
		/// </summary>
		const std::string& m_texturesDirectory;

		/// <summary>
		/// The map storing all meshes by their unique ID.
		/// </summary>
		std::map<int, std::unique_ptr<Mesh>> m_meshType2MeshMap;

		///// <summary>
		///// The map storing all instance meshes by their unique ID.
		///// </summary>
		//std::map<int, InstanceMesh> m_meshType2InstanceMeshMap;

		///// <summary>
		///// The map storing all terrain meshes by their unique ID.
		///// </summary>
		//std::map<int, TerrainMesh> m_meshType2TerrainMeshMap;

		///// <summary>
		///// The map storing all billboard meshes by their unique ID.
		///// </summary>
		//std::map<int, BillboardMesh> m_meshType2BillboardMeshMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshFactory */

} /* end namespace Rendering */

#endif /* __RENDERING_MESH_FACTORY_H__ */