#ifndef __RENDERING_SPOT_LIGHT_H__
#define __RENDERING_SPOT_LIGHT_H__

#include "PointLight.h"
#include "Attenuation.h"

#include "Math/Angle.h"
#include "Math/Transform.h"

namespace rendering {
	namespace lighting
	{
		/// <summary>
		/// The representation of the spot light in the rendering library.
		/// The spot light is a strong beam of light that illuminates only a small area, used especially to center attention on a stage performer.
		/// The spot light is emitted through a cone and by modifying the cone's size one can change the illuminated area to one's liking.
		/// </summary>
		class SpotLight : public PointLight
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API SpotLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderID,
				int terrainShaderID, int noShadowShaderID, int noShadowTerrainShaderID,
				const Attenuation& attenuation, math::Real shadowInfoProjectionNearPlane, bool shadowInfoFlipFacesEnabled,
				int shadowInfoShadowMapSizeAsPowerOf2, math::Real shadowInfoShadowSoftness, math::Real shadowInfoLightBleedingReductionFactor,
				math::Real shadowInfoMinVariance, const math::Angle& viewAngle);

			/// <summary> Spot light destructor. </summary>
			RENDERING_API virtual ~SpotLight();

			/// <summary> Spot light copy constructor. </summary>
			/// <param name="spotLight"> The reference to spot light to copy construct from. </param>
			SpotLight(const SpotLight& spotLight) = delete;

			/// <summary> Spot light move constructor. </summary>
			/// <param name="spotLight"> The r-value reference to spot light to move assign from. </param>
			RENDERING_API SpotLight(SpotLight&& spotLight) = default;

			/// <summary> Spot light copy assignment operator. </summary>
			/// <param name="spotLight"> The reference to spot light to copy assign from. </param>
			/// <returns> The reference to the newly copu-assigned spot light. </returns>
			SpotLight& operator=(const SpotLight& spotLight) = delete;

			/// <summary> Spot light move assignment operator. </summary>
			/// <param name="spotLight"> The r-value reference to spot light to move assign from. </param>
			/// <returns> The reference to the newly move-assigned spot light. </returns>
			RENDERING_API SpotLight& operator=(SpotLight&& spotLight) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Returns a value of a cosine (so it is in range between <code>-1</code> and <code>1</code>) of the angle
			/// between light direction and the spot light's cone edge.
			/// If the value of the cosine between a light direction and spot light direction vectors is
			/// smaller than cutoff then the point is not in the spot light cone.
			/// If <code>cutoff == 0.0</code> then field of view is equal to 180 degrees.
			/// If <code>cutoff == 1.0</code> then field of view equals 0 degrees.
			/// </summary>
			/// <returns> The cosine value of the view angle of the spot light. </returns>
			RENDERING_API math::Real GetCutoff() const noexcept { return m_cutoff; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>
			/// Stores a value of a cosine (so it is in range between <code>-1</code> and <code>1</code>) of the angle
			/// between light direction and the spot light's cone edge.
			/// If the value of the cosine between a light direction and spot light direction vectors is
			/// smaller than cutoff then the point is not in the spot light cone.
			/// If <code>cutoff == 0.0</code> then field of view is equal to 180 degrees.
			/// If <code>cutoff == 1.0</code> then field of view equals 0 degrees.
			/// </summary>
			math::Real m_cutoff;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SpotLight */

	} /* end namespace lighting */

} /* end namespace rendering */

#endif /* __RENDERING_SPOT_LIGHT_H__ */