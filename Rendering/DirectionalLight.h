#ifndef __RENDERING_DIRECTIONAL_LIGHT_H__
#define __RENDERING_DIRECTIONAL_LIGHT_H__

#include "Rendering.h"
#include "BaseLight.h"
#include "Math\Transform.h"
#include "Math\Angle.h"

namespace Rendering {
	namespace Lighting
	{

		/// <summary>
		/// The directional light representation.
		/// </summary>
		class DirectionalLight : public BaseLight
		{
			/* ==================== Static variables and functions begin ==================== */
		private:
			static bool directionalLightsEnabled;
		public:
			static bool* GetDirectionalLightsEnabled() { return &directionalLightsEnabled; }

			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API DirectionalLight();
			RENDERING_API virtual ~DirectionalLight(void);
			
			/// <summary> Directional light copy constructor. </summary>
			DirectionalLight(const DirectionalLight& directionalLight) = default;
			/// <summary> Directional light move constructor. </summary>
			RENDERING_API DirectionalLight(DirectionalLight&& directionalLight) = default;
			/// <summary> Directional light copy assignment operator. </summary>
			DirectionalLight& operator=(const DirectionalLight& directionalLight) = delete;
			/// <summary> Directional light move assignment operator. </summary>
			RENDERING_API DirectionalLight& operator=(DirectionalLight&& directionalLight) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }

			RENDERING_API virtual bool IsEnabled() const;

			RENDERING_API virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot) const;

			RENDERING_API void SetShadowInfo(Math::Real halfShadowArea, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness,
				Math::Real lightBleedingReductionAmount, Math::Real minVariance);

			RENDERING_API std::string ToString() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_halfShadowArea;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_DIRECTIONAL_LIGHT_H__ */