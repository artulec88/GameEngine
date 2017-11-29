#ifndef __ENGINE_INPUT_RANGE_CONVERTER_H__
#define __ENGINE_INPUT_RANGE_CONVERTER_H__

#include "Engine.h"
#include "ActionConstants.h"

#include "Math/Math.h"
#include "Math\Interpolation.h"
//#include "Math\Interpolation_impl.h"

#include "Utility/ILogger.h"

#include <string>
#include <map>

namespace engine
{
	namespace input
	{
		/// <summary>
		/// The input range converter.
		/// </summary>
		class InputRangeConverter
		{
			struct Converter
			{
				Converter() :
					m_minInput(REAL_ZERO),
					m_maxInput(REAL_ONE),
					m_minOutput(REAL_ZERO),
					m_maxOutput(REAL_ONE)
				{
				}

				Converter(math::Real minInput, math::Real maxInput, math::Real minOutput, math::Real maxOutput) :
					m_minInput(minInput),
					m_maxInput(maxInput),
					m_minOutput(minOutput),
					m_maxOutput(maxOutput)
				{
				}

				template <typename RangeType>
				RangeType Convert(RangeType inputValue) const
				{
					math::Real v = static_cast<math::Real>(inputValue);
					DELOCUST_LOG_ENGINE("v = ", v, "; minInput = ", m_minInput, "; maxInput = ", m_maxInput, "; minOutput = ", m_minOutput, "; maxOutput = ", m_maxOutput);
					if (v < m_minInput)
					{
						v = m_minInput;
					}
					else if (v > m_maxInput)
					{
						v = m_maxInput;
					}
					return math::interpolation::InterpolateLinear<RangeType>(m_minOutput, m_maxOutput, (v - m_minInput) / (m_maxInput - m_minInput));
				}

				math::Real m_minInput;
				math::Real m_maxInput;
				math::Real m_minOutput;
				math::Real m_maxOutput;
			}; /* end struct Converter */
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			explicit InputRangeConverter(const std::string& inputContextName);
			explicit InputRangeConverter(std::ifstream& inFileStream);
			~InputRangeConverter();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			template <typename RangeType>
			RangeType Convert(ranges::Range rangeId, RangeType inputValue) const
			{
				const auto itr = m_convertersMap.find(rangeId);
				CHECK_CONDITION_RETURN_ALWAYS_ENGINE(itr != m_convertersMap.end(), inputValue, utility::logging::WARNING,
					"There exists no input converter for the range ", rangeId, ".");
				return itr->second.Convert<RangeType>(inputValue);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::map<ranges::Range, Converter> m_convertersMap;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class InputRangeConverter */

	} /* end namespace input */

} /* end namespace engine */

#endif // __ENGINE_INPUT_RANGE_CONVERTER_H__
