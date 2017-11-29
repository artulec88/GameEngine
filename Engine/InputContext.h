#ifndef __ENGINE_INPUT_CONTEXT_H__
#define __ENGINE_INPUT_CONTEXT_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"
#include "InputRangeConverter.h"
#include <string>
#include <map>
#include <memory>
#include <fstream>

namespace engine
{
	namespace input
	{
		/// <summary>
		/// The input context.
		/// </summary>
		class InputContext
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			explicit InputContext(const std::string& inputContextName);
			explicit InputContext(std::ifstream& inputContextFile);
			~InputContext();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Attempts to map a raw button to an action. If it fails <code>Actions::INVALID</code> is returned.
			/// </summary>
			/// <param name="button">The raw button for which we want to find an associated action. </param>
			/// <returns>
			/// An action that is associated with the specified <paramref name="button"/>. If no such action is found then <code>Actions::INVALID</code> is returned.
			/// </returns>
			actions::Action MapButtonToAction(raw_input_keys::RawInputKey button) const;
			/// <summary>
			/// Attempts to map a raw button to a state. If it fails <code>States::STATE_INVALID</code> is returned.
			/// </summary>
			/// <param name="button">The raw button for which we want to find an associated state. </param>
			/// <returns>
			/// A state that is associated with the specified <paramref name="button"/>. If no such state is found then <code>States::STATE_INVALID</code> is returned.
			/// </returns>
			states::State MapButtonToState(raw_input_keys::RawInputKey button) const;
			/// <summary>
			/// Attempts to map a raw axis to a range. If it fails <code>Ranges::RANGE_INVALID</code> is returned.
			/// </summary>
			/// <param name="button">The raw axis for which we want to find an associated range. </param>
			/// <returns>
			/// A range that is associated with the specified <paramref name="axis"/>. If no such range is found then <code>Ranges::RANGE_INVALID</code> is returned.
			/// </returns>
			ranges::Range MapAxisToRange(RawInputAxes::RawInputAxis axis) const;

			/// <summary>
			/// Retrieves the sensitivity associated with a given <paramref name="range"/>.
			/// </summary>
			/// <param name="range"> The range we want to get sensitivity for. </param>
			/// <returns>
			/// The sensitivity associated with a specified <paramref name="range"/>.
			/// </returns>
			math::Real GetSensitivity(ranges::Range range) const;

			const InputRangeConverter& GetConverter() const { return *m_converter; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::map<raw_input_keys::RawInputKey, actions::Action> m_actionsMap;
			std::map<raw_input_keys::RawInputKey, states::State> m_statesMap;
			std::map<RawInputAxes::RawInputAxis, ranges::Range> m_rangesMap;

			std::map<ranges::Range, math::Real> m_sensitivitiesMap;
			std::unique_ptr<InputRangeConverter> m_converter;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class InputContext */

	} /* end namespace input */

} /* end namespace engine */

#endif // __ENGINE_INPUT_CONTEXT_H__
