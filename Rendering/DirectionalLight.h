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
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Directional light constructor. </summary>
			/// <param name="transform"> The transform of the directiona light. </param>
			/// <param name="color"> The color this directional light will emit. </param>
			/// <param name="intensity"> The intensity of this directional light. </param>
			RENDERING_API DirectionalLight(const Math::Transform& transform, const Color& color, Math::Real intensity);

			/// <summary> Directional light destructor. </summary>
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

			RENDERING_API virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot) const;

			RENDERING_API void SetShadowInfo(Math::Real halfShadowArea, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness,
				Math::Real lightBleedingReductionAmount, Math::Real minVariance);

			friend std::ostream& operator<<(std::ostream& out, const DirectionalLight& directionalLight)
			{
				out << "Directional light: " << std::endl << "(Intensity=" << directionalLight.m_intensity << "; Color=" <<
					directionalLight.m_color << "; Direction=" << directionalLight.GetTransform().GetTransformedRot().GetForward() << ")";
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_halfShadowArea;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_DIRECTIONAL_LIGHT_H__ */