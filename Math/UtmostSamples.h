#ifndef __MATH_UTMOST_TIME_SAMPLES_H__
#define __MATH_UTMOST_TIME_SAMPLES_H__

#include "Math.h"

#include <string>
#include <array>

namespace math {
	namespace statistics
	{

		/// <summary>The class responsible for maintaining the list of utmost observations in the samples.</summary>
		/// <remarks>
		///	For example, for a given time samples: 0.1s, 0.2s, 0.5s, 1.0s, 2.0s, 5.0s, 10.0s, 20.0s, 50.0s and number of samples equal to 3
		/// the object will store 3 minimum and 3 maximum samples. In this case it will be (0.1s, 0.2s, 0.5s) for minimum observations and (10.0s, 20.0s, 50.0s) for maximum observations.
		/// </remarks>
		template <class T>
		class UtmostSamples
		{
			/* ==================== Static variables and functions begin ==================== */
		private:
			static constexpr int NUMBER_OF_SAMPLES = 3;
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			UtmostSamples();
			~UtmostSamples();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			void ProcessSample(const T& sample);
			std::string ToString();
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::array<T, NUMBER_OF_SAMPLES> m_minSamples;
			std::array<T, NUMBER_OF_SAMPLES> m_maxSamples;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class UtmostTimeSamples */

	} /* end namespace statistics */

} /* end namespace math */

#endif /* __MATH_UTMOST_TIME_SAMPLES_H__ */