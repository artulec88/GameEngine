#ifndef __RENDERING_TERRAIN_H__
#define __RENDERING_TERRAIN_H__

#include "Rendering.h"

namespace Rendering
{

	/// <summary>The representation of terrain in the rendering engine.</summary>
	class Terrain
	{
	private:
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Terrain constructor. </summary>
		RENDERING_API Terrain(int meshID);

		/// <summary> Terrain destructor. </summary>
		RENDERING_API ~Terrain();

		/// <summary> Terrain copy constructor. </summary>
		/// <param name="terrain"> The reference to the terrain we want to copy construct new terrain from. </param>
		RENDERING_API Terrain(const Terrain& terrain) = delete;

		/// <summary> Terrain move constructor. </summary>
		/// <param name="terrain"> The r-value reference to the terrain we want to move construct new terrain from. </param>
		RENDERING_API Terrain(Terrain&& terrain) = delete;

		/// <summary> Terrain copy assignment operator. </summary>
		/// <param name="terrain"> The reference to the terrain we want to copy assign new terrain from. </param>
		/// <returns> The reference to the copy-assigned terrain. </returns>
		RENDERING_API Terrain& operator=(const Terrain& terrain) = delete;

		/// <summary> Terrain move assignment operator. </summary>
		/// <param name="terrain"> The r-value reference to the terrain we want to move assign new terrain from. </param>
		/// <returns> The reference to the move-assigned terrain. </returns>
		RENDERING_API Terrain& operator=(Terrain&& terrain) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		//RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz) const { return GetHeightAt(xz.x, xz.y); }
		//RENDERING_API Math::Real GetHeightAt(Math::Real x, Math::Real y) const;
		//RENDERING_API void TransformPositions(const Math::Matrix4D& transformationMatrix);
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The ID of the mesh this terrain uses.
		/// </summary>
		int m_meshID;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Terrain */

} /* end namespace Rendering */

#endif /* __RENDERING_TERRAIN_H__ */