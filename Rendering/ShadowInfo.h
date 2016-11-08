#ifndef __RENDERING_SHADOW_INFO_H__
#define __RENDERING_SHADOW_INFO_H__

#include "Rendering.h"
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

namespace Rendering
{
	/// <summary>
	/// 
	/// </summary>
	struct ShadowCameraTransform
	{
		ShadowCameraTransform(const Math::Vector3D& position, const Math::Quaternion& rotation) :
			m_pos(position),
			m_rot(rotation)
		{
		}

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
	};

	/// <summary>
	/// Stores all information a light needs to cast shadows.
	/// </summary>
	/// <remarks>
	/// Watch https://www.youtube.com/watch?v=CAMvRfN14r0&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl&index=9&t=289s.
	/// </remarks>
	class ShadowInfo
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Shadow info constructor.
		/// </summary>
		/// <param name="projection">
		/// The projection matrix that is used by the camera when we render the shadow map.
		/// Different lights can use different kinds of shadow projection matrices, e.g. directional light uses orthogonal projection matrix, because all its rays go in parallel to each other.
		/// For spot lights though we will need a special projection matrix, because the spot light casts shadow in a cone.
		/// </param>
		/// <param name="flipFacesEnabled"><code>true</code> when </param>
		/// <param name="shadowMapSizeAsPowerOf2"> The size of the shadow map the light uses. </param>
		/// <param name="shadowSoftness"> The softness of the shadow. </param>
		/// <param name="lightBleedingReductionFactor"> The light bleeding reduction factor. </param>
		/// <param name="minVariance"></param>
		ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled,
			int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness = REAL_ONE,
			Math::Real lightBleedingReductionFactor = static_cast<Math::Real>(0.2f),
			Math::Real minVariance = static_cast<Math::Real>(0.00002f));

		//ShadowInfo(Math::Matrix4D&& projection, bool flipFacesEnabled,
		//	int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness = REAL_ONE,
		//	Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
		//	Math::Real minVariance = static_cast<Math::Real>(0.00002f));

		/// <summary> Shadow info destructor. </summary>
		~ShadowInfo();

		/// <summary> Shadow info copy constructor. </summary>
		/// <param name="shadowInfo"> The shadow info to copy construct from. </param>
		ShadowInfo(const ShadowInfo& shadowInfo) = delete;

		/// <summary> Shadow info move constructor. </summary>
		/// <param name="shadowInfo"> The shadow info to move construct from. </param>
		ShadowInfo(ShadowInfo&& shadowInfo) = delete;

		/// <summary> Shadow info copy assignment operator. </summary>
		/// <param name="shadowInfo"> The shadow info to copy assign from. </param>
		/// <returns> The newly copy-assigned shadow info object. </returns>
		ShadowInfo& operator=(const ShadowInfo& shadowInfo) = delete;

		/// <summary> Shadow info move assignment operator. </summary>
		/// <param name="shadowInfo"> The shadow info to move assign from. </param>
		/// <returns> The newly move-assigned shadow info object. </returns>
		ShadowInfo& operator=(ShadowInfo&& shadowInfo) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Gets the projection matrix that shadow info uses. </summary>
		/// <returns> The projection matrix that shadow info uses. </returns>
		RENDERING_API inline const Math::Matrix4D& GetProjection() const noexcept { return m_projectionMatrix; }

		/// <summary> Gets the shadow softness value. </summary>
		/// <returns> The shadow softness value. </returns>
		RENDERING_API inline Math::Real GetShadowSoftness() const noexcept { return m_shadowSoftness; }

		/// <summary> Gets the minimal variance of the shadow info. </summary>
		/// <returns> The minimal variance of the shadow info. </returns>
		RENDERING_API inline Math::Real GetMinVariance() const noexcept { return m_minVariance; }

		/// <summary> Gets the light bleeding reduction factor. </summary>
		/// <returns> The light bleeding reduction factor. </returns>
		RENDERING_API inline Math::Real GetLightBleedingReductionFactor() const noexcept { return m_lightBleedingReductionFactor; }

		/// <summary> Gets the information whether or not the rendering engine should first turn on backface culling before rendering to the shadow map. </summary>
		/// <returns> <code>true</code> when before rendering to shadow map we should turn on backface culling. Otherwise <code>false</code>. </returns>
		RENDERING_API inline bool IsFlipFacesEnabled() const noexcept { return m_flipFacesEnabled; }

		/// <summary> Gets the exponent of the final shadow map size. </summary>
		/// <returns> The exponent of the final shadow map size. </returns>
		RENDERING_API inline int GetShadowMapSizeAsPowerOf2() const noexcept { return m_shadowMapSizeAsPowerOf2; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The projection matrix used by the camera when rendering to the shadow map
		/// (e.g. for directional light this is an orthogonal projection matrix, because all its rays go in parallel to one another).
		/// </summary>
		Math::Matrix4D m_projectionMatrix;

		/// <summary>
		/// Stores the information whether or not we are supposed to flip the culling faces (from back to front and vice versa) before rendering to the shadow map.
		/// This way we can overcome the "Peter Panning" problem (see http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/).
		/// Due to the non-infinite resolution of the shadow map the pixels right near to the object casting the shadow tend to be incorrectly lit as if they were not in shadow.
		/// This makes the object look as if it were flying (hence the term "Peter Panning"). If we enable backface culling then there will be a polygon of the object which faces
		/// the light, which will occlude the other side, which wouldn't normally be rendered without backface culling. The drawback is that we have twice as many triangles to render.
		/// </summary>
		bool m_flipFacesEnabled;

		/// <summary>
		/// The exponent that determines the size of the shadow map the shadow info uses. The shadow map size is equal to <code>2^(m_shadowMapSizeAsPowerOf2)</code>.
		/// </summary>
		int m_shadowMapSizeAsPowerOf2;
		Math::Real m_shadowSoftness;
		Math::Real m_lightBleedingReductionFactor;
		Math::Real m_minVariance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShadowInfo */

} /* end namespace Rendering */

#endif /* __RENDERING_SHADOW_INFO_H__ */