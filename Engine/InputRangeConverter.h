#ifndef __ENGINE_INPUT_RANGE_CONVERTER_H__
#define __ENGINE_INPUT_RANGE_CONVERTER_H__

#include "Engine.h"
#include "ActionConstants.h"

#include "Math/Math.h"
#include "Math/Interpolation.h"
#include "Math/Interpolation_impl.h"

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
					minInput(REAL_ZERO),
					maxInput(REAL_ONE),
					minOutput(REAL_ZERO),
					maxOutput(REAL_ONE)
				{
				}

				Converter(math::Real minInput, math::Real maxInput, math::Real minOutput, math::Real maxOutput) :
					minInput(minInput),
					maxInput(maxInput),
					minOutput(minOutput),
					maxOutput(maxOutput)
				{
				}

				template <typename RangeType>
				RangeType Convert(RangeType inputValue) const
				{
					auto v = static_cast<math::Real>(inputValue);
					DELOCUST_LOG_ENGINE("v = ", v, "; minInput = ", minInput, "; maxInput = ", maxInput, "; minOutput = ", minOutput, "; maxOutput = ", maxOutput);
					if (v < minInput)
					{
						v = minInput;
					}
					else if (v > maxInput)
					{
						v = maxInput;
					}
					return math::interpolation::InterpolateLinear<RangeType>(minOutput, maxOutput, (v - minInput) / (maxInput - minInput));
				}

				math::Real minInput;
				math::Real maxInput;
				math::Real minOutput;
				math::Real maxOutput;
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
