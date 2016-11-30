#ifndef __RENDERING_TERRAIN_H__
#define __RENDERING_TERRAIN_H__

#include "Rendering.h"
#include "Mesh.h"

#include "Math\KDTree.h"

namespace Rendering
{

	class Terrain
	{
		/* ==================== Static variables begin ==================== */
	private:
		/// <summary> The size of the single squared-size terrain tile. </summary>
		static constexpr int SIZE = 400;
		/// <summary>
		/// The maximal height of the terrain. The negative value represents the minimal height of the terrain.
		/// In other words, the terrain height always lies in range [-HEIGHTMAP_MAX_HEIGHT; HEIGHTMAP_MAX_HEIGHT].
		/// </summary>
		static constexpr Math::Real HEIGHTMAP_MAX_HEIGHT = 5.0f;
		/// <summary> The maximum value for color of the single pixel in the height map. </summary>
		static constexpr Math::Real MAX_PIXEL_COLOR = 255.0f;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="fileName"> The file storing the model for the terrain mesh. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		explicit Terrain(const std::string& fileName, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="gridX"> The X position on the grid. </param>
		/// <param name="gridZ"> The Z position on the grid. </param>
		/// <param name="heightMapFileName">
		/// The height map file name. The height map is a grey-scale texture where the brighter the color the higher the respective point on the terrain will be.
		/// </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		//Terrain(int gridX, int gridZ, const std::string& heightMapFileName, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="gridX"> The X position on the grid. </param>
		/// <param name="gridZ"> The Z position on the grid. </param>
		/// <param name="heightsGenerator"> The heights generator. </param>
		/// <param name="vertexCount"> The number of vertices for the terrain. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		//Terrain(int gridX, int gridZ, const Math::HeightsGenerator& heightsGenerator, int vertexCount, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh destructor. </summary>
		virtual ~Terrain(void);
		/// <summary> Terrain mesh copy constructor. </summary>
		Terrain(const Terrain& Terrain) = delete;
		/// <summary> Terrain mesh move constructor. </summary>
		Terrain(Terrain&& Terrain) = delete;
		/// <summary> Terrain mesh copy assignment operator. </summary>
		Terrain& operator=(const Terrain& Terrain) = delete;
		/// <summary> Terrain mesh move assignment operator. </summary>
		Terrain& operator=(Terrain&& Terrain) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		//RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz) const { return GetHeightAt(xz.x, xz.y); }
		//RENDERING_API Math::Real GetHeightAt(Math::Real x, Math::Real y) const;
		//RENDERING_API void TransformPositions(const Math::Matrix4D& transformationMatrix);
	private:
		//int GetHeightMapIndex(int x, int z) const;
		//Math::Real CalculateHeightAt(int x, int z, unsigned char* heightMapData);
		//Math::Real CalculateHeightAt(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		//Math::Vector3D CalculateNormal(int x, int z, unsigned char* heightMapData);
		//Math::Vector3D CalculateNormal(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		/* ==================== Non-static member functions end ==================== */


		/* ==================== Non-static member variables begin ==================== */
	private:
		const Mesh* m_terrainMesh;

		/// <summary> The position of the single terrain tile along the X axis. </summary>
		int m_x;
		/// <summary> The position of the single terrain tile along the Z axis. </summary>
		int m_z;

		int m_vertexCount;
#ifdef HEIGHTS_KD_TREE
		std::vector<Math::Vector3D> m_positions;
		const unsigned int m_kdTreeSamples;
		std::unique_ptr<Math::KDTree> m_kdTree;
#elif defined HEIGHTS_HEIGHTMAP
		int m_heightMapWidth, m_heightMapHeight;
		std::vector<Math::Real> m_heights;
		Math::Real m_gridSquareSize;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Terrain */

} /* end namespace Rendering */

#endif /* __RENDERING_TERRAIN_H__ */