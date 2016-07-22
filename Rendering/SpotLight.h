#pragma once

#include "PointLight.h"
#include "Attenuation.h"

#include "Math\Vector.h"
#include "Math\Angle.h"
#include "Math\Transform.h"

namespace Rendering {
	namespace Lighting
	{
		/// <summary>
		/// The representation of the spot light in the rendering library.
		/// The spot light is a strong beam of light that illuminates only a small area, used especially to center attention on a stage performer.
		/// The spot light is emitted through a cone and by modifying the cone's size one can change the illuminated area to one's liking.
		/// </summary>
		class SpotLight : public PointLight
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			static bool spotLightsEnabled;
		public:
			static bool* GetSpotLightsEnabled() { return &spotLightsEnabled; }
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API SpotLight(Math::Transform& transform, const Color& color, Math::Real intensity, const Attenuation& attenuation,
				const Math::Angle& viewAngle, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance,
				const Shader& shader, const Shader& terrainShader, const Shader& noShadowShader, const Shader& noShadowTerrainShader);
			RENDERING_API virtual ~SpotLight(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
			Math::Real GetCutoff() const { return m_cutoff; };
			//virtual void InitializeShaders();
			virtual bool IsEnabled() const;
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
			Math::Real m_cutoff;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SpotLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */