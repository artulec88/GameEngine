#ifndef __RENDERING_DIRECTIONAL_LIGHT_H__
#define __RENDERING_DIRECTIONAL_LIGHT_H__

#include "Rendering.h"
#include "BaseLight.h"
#include "Math/Transform.h"
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
			/// <param name="shaderID"> The ID of the directional light shader for all standard meshes. </param>
			/// <param name="terrainShaderID"> The ID of the directional light shader for terrain meshes. </param>
			/// <param name="noShadowShaderID"> The ID of the directional light shader for all standard meshes without shadow casting calculation. </param>
			/// <param name="noShadowTerrainShaderID"> The ID of the directional light shader for terrain meshes without shadow casting calculation. </param>
			/// <param name="shadowInfoFlipFacesEnabled"> The information whether the backface culling should be enabled for rendering the light or not. </param>
			/// <param name="shadowInfoShadowMapSizeAsPowerOf2"> The value of the exponent in the equation for calculating the shadow map size to be used by the directional light. </param>
			/// <param name="shadowInfoShadowSoftness"> The softness of the directiona light shadow. </param>
			/// <param nane="shadowInfoLightBleedingReductionFactor"> The light bleeding reduction factor. </param>
			/// <param name="shadowInfoMinVariance"> The minimal variance in Variance Shadow Mapping calculation for the directional light. </param>
			/// <param name="halfShadowArea"> The half shadow area for the directiona light. </param>
			RENDERING_API DirectionalLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderID,
				int terrainShaderID, int noShadowShaderID, int noShadowTerrainShaderID,
				bool shadowInfoFlipFacesEnabled, int shadowInfoShadowMapSizeAsPowerOf2, math::Real shadowInfoShadowSoftness,
				math::Real shadowInfoLightBleedingReductionFactor, math::Real shadowInfoMinVariance, math::Real halfShadowArea);

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
			//math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }

			RENDERING_API virtual ShadowCameraTransform CalcShadowCameraTransform(const math::Vector3D& cameraPos, const math::Quaternion& cameraRot) const;

			friend std::ostream& operator<<(std::ostream& out, const DirectionalLight& directionalLight)
			{
				out << "Directional light: " << std::endl << "(Intensity=" << directionalLight.m_intensity << "; Color=" <<
					directionalLight.m_color << "; Direction=" << directionalLight.GetTransform().GetTransformedRot().GetForward() << ")";
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Real m_halfShadowArea;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_DIRECTIONAL_LIGHT_H__ */