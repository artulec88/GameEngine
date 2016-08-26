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
			RENDERING_API DirectionalLight(const Shader& shader, const Shader& terrainShader,
				const Shader& noShadowShader, const Shader& noShadowTerrainShader);
			RENDERING_API virtual ~DirectionalLight(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }

			virtual bool IsEnabled() const;

			virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);

			RENDERING_API void SetShadowInfo(Math::Real halfShadowArea, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness,
				Math::Real lightBleedingReductionAmount, Math::Real minVariance);

			std::string ToString() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_halfShadowArea;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_DIRECTIONAL_LIGHT_H__ */