#ifndef __RENDERING_FOG_INFO_H__
#define __RENDERING_FOG_INFO_H__

#include "Rendering.h"
#include "Color.h"

namespace rendering
{
	namespace fog_effect
	{
		/// <summary>
		/// Stores all information about the fog.
		/// </summary>
		class FogInfo
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Fog information object constructor.
			/// </summary>
			/// <param name="fogColor"> The color of the fog. </param>
			/// <param name="fogStartDistance"> The value that represents the distance from the camera where fog begins to appear. Used only when <code>LINEAR</code> fall-off type is chosen.</param>
			/// <param name="fogEndDistance"> The value that represents the distance from the camera where fog reaches its maximum density. Used only when <code>LINEAR</code> fall-off type is chosen.</param>
			/// <param name="fogDensityFactor"> The density factor of the fog. The value represents the thickness of the fog. </param>
			/// <param name="fogGradient"> The value representing how fast the fog reaches its maximum density. Used only when <code>EXPONENTIAL</code> fall-off type is chosen.</param>
			/// <param name="fallOffType"> The fall-off type for the fog. </param>
			/// <param name="calculationType"> The calculation type of the fog. </param>
			/// <param name="isEnabled"><code>true</code> when specified fog is enabled and <code>false</code> otherwise.</param>
			explicit FogInfo(Color fogColor = Color(color_ids::GAINSBORO), math::Real fogStartDistance = 8.0f, math::Real fogEndDistance = 50.0f, math::Real fogDensityFactor = 0.2f,
				math::Real fogGradient = 0.005f, FogFallOffType fogFallOffType = EXPONENTIAL, FogCalculationType fogCalculationType = RANGE_BASED, bool isEnabled = true) noexcept;
			
			/// <summary> Fog information destructor. </summary>
			~FogInfo() = default;

			/// <summary>
			/// Fog information copy constructor.
			/// </summary>
			/// <param name="fogInfo"> The fog information to copy construct from. </param>
			FogInfo(const FogInfo& fogInfo) noexcept = default;

			/// <summary>
			/// Fog information move constructor.
			/// </summary>
			/// <param name="fogInfo"> The r-value reference of the fog information to move construct from. </param>
			FogInfo(FogInfo&& fogInfo) noexcept = default;

			/// <summary>
			/// Fog information copy assignment operator.
			/// </summary>
			/// <param name="fogInfo"> The fog information to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned fog information object. </returns>
			FogInfo& operator=(const FogInfo& fogInfo) noexcept = default;

			/// <summary>
			/// Fog information move assignment operator.
			/// </summary>
			/// <param name="fogInfo"> The r-value reference of the fog information to move assign from. </param>
			/// <returns> The reference to the newly move-assigned fog information object. </returns>
			FogInfo& operator=(FogInfo&& fogInfo) noexcept = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API const Color& GetColor() const { return m_color; }
			RENDERING_API math::Real GetStartDistance() const { return m_startDistance; }
			RENDERING_API math::Real GetEndDistance() const { return m_endDistance; }
			RENDERING_API math::Real GetDensityFactor() const { return m_densityFactor; }
			RENDERING_API math::Real GetGradient() const { return m_gradient; }
			RENDERING_API FogFallOffType GetFallOffType() const { return m_fallOffType; }
			RENDERING_API FogCalculationType GetCalculationType() const { return m_calculationType; }
			RENDERING_API bool IsEnabled() const { return m_isEnabled; }
			bool operator<(const FogInfo& fogInfo) const;

#ifdef ANT_TWEAK_BAR_ENABLED
			Color* GetColorPtr() { return &m_color; }
			math::Real* GetStartDistancePtr() { return &m_startDistance; }
			math::Real* GetEndDistancePtr() { return &m_endDistance; }
			math::Real* GetDensityFactorPtr() { return &m_densityFactor; }
			math::Real* GetGradientPtr() { return &m_gradient; }
			FogFallOffType* GetFallOffTypePtr() { return &m_fallOffType; }
			FogCalculationType* GetCalculationTypePtr() { return &m_calculationType; }
			bool* IsEnabledPtr() { return &m_isEnabled; }
#endif
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary> Fog color. </summary>
			Color m_color;

			/// <summary> Represents the distance where fog begins to appear. </summary>
			math::Real m_startDistance;
			/// <summary> Represents the distance where fog ceases to appear. </summary>
			math::Real m_endDistance;

			/// <summary> Represents the thickness of the fog. Increasing this value will generally decrease the visibility of the scene. </summary>
			math::Real m_densityFactor;

			/// <summary>
			/// Represents how fast the transition between no-fog and complete fog is performed.
			/// In other words it determines how quickly the visibility of the scene decreases with distance.
			/// </summary>
			math::Real m_gradient;

			/// <summary> The fall-off type of the fog. </summary>
			FogFallOffType m_fallOffType;

			/// <summary> The calculation type of the fog. </summary>
			FogCalculationType m_calculationType;

			/// <summary> <code>true</code> when fog is enabled and <code>false</code> otherwise. </summary>
			bool m_isEnabled;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class FogInfo */
	} /* end namespace fog_effect */
} /* end namespace rendering */

#endif /* __RENDERING_FOG_INFO_H__ */
