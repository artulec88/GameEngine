#ifndef __ENGINE_INPUT_RANGE_CONVERTER_H__
#define __ENGINE_INPUT_RANGE_CONVERTER_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"

#include "Math\Math.h"
#include "Math\Interpolation.h"
//#include "Math\Interpolation_impl.h"

#include "Utility\ILogger.h"

#include <string>
#include <map>
#include <memory>

namespace engine
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
				Converter() :
					m_minInput(REAL_ZERO),
					m_maxInput(REAL_ONE),
					m_minOutput(REAL_ZERO),
					m_maxOutput(REAL_ONE)
				{
				}

				Converter(Math::Real minInput, Math::Real maxInput, Math::Real minOutput, Math::Real maxOutput) :
					m_minInput(minInput),
					m_maxInput(maxInput),
					m_minOutput(minOutput),
					m_maxOutput(maxOutput)
				{
				}

				template <typename RangeType>
				RangeType Convert(RangeType inputValue) const
				{
					Math::Real v = static_cast<Math::Real>(inputValue);
					DELOCUST_LOG_ENGINE("v = ", v, "; minInput = ", m_minInput, "; maxInput = ", m_maxInput, "; minOutput = ", m_minOutput, "; maxOutput = ", m_maxOutput);
					if (v < m_minInput)
					{
						v = m_minInput;
					}
					else if (v > m_maxInput)
					{
						v = m_maxInput;
					}
					return Math::Interpolation::InterpolateLinear<RangeType>(m_minOutput, m_maxOutput, (v - m_minInput) / (m_maxInput - m_minInput));
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
			explicit InputRangeConverter(const std::string& inputContextName);
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
					WARNING_LOG_ENGINE("There exists no input converter for the range ", rangeId, ".");
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
