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
		/// <param name="lightBleedingReductionAmount"></param>
		/// <param name="minVariance"></param>
		ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled,
			int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness = REAL_ONE,
			Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
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
		RENDERING_API inline const Math::Matrix4D& GetProjection() const { return m_projectionMatrix; }
		RENDERING_API inline Math::Real GetShadowSoftness() const { return m_shadowSoftness; }
		RENDERING_API inline Math::Real GetMinVariance() const { return m_minVariance; }
		RENDERING_API inline Math::Real GetLightBleedingReductionAmount() const { return m_lightBleedingReductionAmount; }
		RENDERING_API inline bool IsFlipFacesEnabled() const { return m_flipFacesEnabled; }
		RENDERING_API inline int GetShadowMapSizeAsPowerOf2() const { return m_shadowMapSizeAsPowerOf2; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The projection matrix used by the camera when rendering to the shadow map
		/// (e.g. for directional light this is an orthogonal projection matrix, because all its rays go in parallel to one another).
		/// </summary>
		Math::Matrix4D m_projectionMatrix;
		bool m_flipFacesEnabled;
		int m_shadowMapSizeAsPowerOf2;
		Math::Real m_shadowSoftness;
		Math::Real m_lightBleedingReductionAmount;
		Math::Real m_minVariance;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShadowInfo */

} /* end namespace Rendering */

#endif /* __RENDERING_SHADOW_INFO_H__ */