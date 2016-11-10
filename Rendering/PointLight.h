#ifndef __RENDERING_POINT_LIGHT_H__
#define __RENDERING_POINT_LIGHT_H__

#include "Rendering.h"
#include "Attenuation.h"
#include "BaseLight.h"
#include "Color.h"

namespace Rendering {
	namespace Lighting
	{

		class PointLight : public BaseLight
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API PointLight(const Math::Transform& transform, const Color& color, Math::Real intensity, int shaderID,
				int terrainShaderID, int noShadowShaderID, int noShadowTerrainShaderID, const Attenuation& attenuation);

			/// <summary> Point light destructor. </summary>
			RENDERING_API virtual ~PointLight(void);

			/// <summary> Point light copy constructor. </summary>
			PointLight(const PointLight& pointLight) = default;
			/// <summary> Point light move constructor. </summary>
			RENDERING_API PointLight(PointLight&& pointLight) = default;
			/// <summary> Point light copy assignment operator. </summary>
			PointLight& operator=(const PointLight& pointLight) = delete;
			/// <summary> Point light move assignment operator. </summary>
			RENDERING_API PointLight& operator=(PointLight&& pointLight) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Gets the attenuation of the point light. </summary>
			/// <returns> The attenuation of the point light. </returns>
			RENDERING_API Attenuation GetAttenuation() const noexcept { return m_attenuation; }
			
			/// <summary> Gets the range of the point light. </summary>
			/// <returns> The range of the point light. </returns>			
			RENDERING_API Math::Real GetRange() const noexcept { return m_range; }

			//RENDERING_API void SetAttenuation(const Attenuation& attenuation)
			//{
			//	m_attenuation = attenuation;
			//	m_range = CalculateRange();
			//}
		private:
			Math::Real CalculateRange();
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			/// <summary> The attenuation of the point light. </summary>
			Attenuation m_attenuation;

			/// <summary> The range of the point light. </summary>
			Math::Real m_range;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PointLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_POINT_LIGHT_H__ */