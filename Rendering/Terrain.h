#ifndef __RENDERING_TERRAIN_H__
#define __RENDERING_TERRAIN_H__

#include "Rendering.h"
#include "Mesh.h"

#include "Math\Transform.h"

//#define HEIGHTS_KD_TREE
#define HEIGHTS_SURFACE

#ifdef HEIGHTS_KD_TREE
#include "Math\KDTree.h"
#elif defined HEIGHTS_SURFACE
#include "Math\Surface.h"
#endif

namespace Rendering
{
	/// <summary>
	/// Rendering engine representation of the terrain.
	/// </summary>
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
		/// <param name="heights"> The terrain mesh positions. </param>
		/// <param name="heightsCount">
		/// Number of mesh positions.
		/// In other words, this value represents how many elements the <paramref name="heights"/> array contains.
		/// </param>
		/// <param name="terrainTransform"> The world transform of the terrain. </param>
		RENDERING_API Terrain(Math::Real* heights, int heightsCount, const Math::Transform& terrainTransform);
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
		RENDERING_API ~Terrain(void);
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
		/// <summary>
		/// Finds and returns the height of the terrain at given location <paramref name="xz"/>.
		/// </summary>
		/// <param name="xz">
		/// The two-dimensional vector representing the location in the world.
		/// The output of the function will represent the height of the terrain at that particular location.
		/// </param>
		/// <returns> The height of the terrain at location <paramref name="xz"/>. </returns>
		RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz) const { return GetHeightAt(xz.x, xz.y); }

		/// <summary>
		/// Finds and returns the height of the terrain at location given by the two specified X and Z components
		/// (<paramref name="x"/> and <paramref name="z"/> respectively).
		/// </summary>
		/// <param name="x">
		/// The X component of the location in the world for which we want to determine the height of the terrain.
		/// </param>
		/// <param name="z">
		/// The Z component of the location in the world for which we want to determine the height of the terrain.
		/// </param>
		/// <returns>
		/// The height of the terrain at location given by the two specified X and Z components
		/// (<paramref name="x"/> and <paramref name="z"/> respectively).
		/// </returns>
		RENDERING_API Math::Real GetHeightAt(Math::Real x, Math::Real y) const;
	private:
		//int GetHeightMapIndex(int x, int z) const;
		//Math::Real CalculateHeightAt(int x, int z, unsigned char* heightMapData);
		//Math::Real CalculateHeightAt(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		//Math::Vector3D CalculateNormal(int x, int z, unsigned char* heightMapData);
		//Math::Vector3D CalculateNormal(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		/* ==================== Non-static member functions end ==================== */


		/* ==================== Non-static member variables begin ==================== */
	private:
		//const Mesh* m_terrainMesh;

		/// <summary> The position of the single terrain tile along the X axis. </summary>
		int m_x;
		/// <summary> The position of the single terrain tile along the Z axis. </summary>
		int m_z;

#ifdef HEIGHTS_KD_TREE
		Math::KDTree m_kdTree;
#elif defined HEIGHTS_SURFACE
		Math::Surface m_surface;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Terrain */

} /* end namespace Rendering */

#endif /* __RENDERING_TERRAIN_H__ */