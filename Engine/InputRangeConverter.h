#ifndef __ENGINE_INPUT_RANGE_CONVERTER_H__
#define __ENGINE_INPUT_RANGE_CONVERTER_H__

#include "Engine.h"
#include "InputConstants.h"

#include "Math\Interpolation.h"
#include "Math\Interpolation_impl.h"

#include <string>
#include <map>
#include <memory>

namespace Engine
{
	namespace Input
	{
		/// <summary>
		/// The input range converter.
		/// </summary>
		class InputRangeConverter
		{
		private:
			struct Converter
			{
				template <typename RangeType>
				RangeType Convert(RangeType inputValue) const
				{
					Math::Real v = static_cast<Math::Real>(inputValue);
					if (v < m_minInput)
					{
						v = m_minInput;
					}
					else if (v > m_maxInput)
					{
						v = m_maxInput;
					}
					Math::Interpolation::LinearInterpolator<RangeType> interpolator;
					return interpolator.Interpolate(m_minOutput, m_maxOutput, (v - m_minInput) / (m_maxInput - m_minInput));
				}

				Math::Real m_minInput;
				Math::Real m_maxInput;
				Math::Real m_minOutput;
				Math::Real m_maxOutput;
			}; /* end struct Converter */
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			explicit InputRangeConverter(std::ifstream& inFileStream);
			~InputRangeConverter();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			template <typename RangeType>
			RangeType Convert(Ranges::Range rangeId, RangeType inputValue) const
			{
				std::map<Ranges::Range, Converter>::const_iterator itr = m_convertersMap.find(rangeId);
				if (itr == m_convertersMap.end())
				{
					return inputValue;
				}
				return itr->second.Convert<RangeType>(inputValue);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::map<Ranges::Range, Converter> m_convertersMap;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class InputRangeConverter */

	} /* end namespace Input */

} /* end namespace Engine */

#endif // __ENGINE_INPUT_RANGE_CONVERTER_H__
