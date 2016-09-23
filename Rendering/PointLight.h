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
		public:
			static bool pointLightsEnabled;
		public:
			RENDERING_API static bool* ArePointLightsEnabled() { return &pointLightsEnabled; }
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API PointLight();
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
			RENDERING_API Attenuation GetAttenuation() const { return m_attenuation; };
			RENDERING_API Math::Real GetRange() const { return m_range; };
			RENDERING_API virtual bool IsEnabled() const;

			RENDERING_API void SetAttenuation(const Attenuation& attenuation)
			{
				m_attenuation = attenuation;
				CalculateRange();
			}
			//RENDERING_API void SetRange(Math::Real range) { m_range = range; }
		private:
			void CalculateRange();
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			Attenuation m_attenuation;
			Math::Real m_range;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PointLight */

	} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_POINT_LIGHT_H__ */