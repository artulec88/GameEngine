#ifndef __ENGINE_INPUT_MAPPING_H__
#define __ENGINE_INPUT_MAPPING_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "InputConstants.h"

#include "Math/Math.h"

#include <set>
#include <list>
#include <map>
#include <memory>

namespace engine
{
	namespace input
	{
		using ActionsContainer = std::set<actions::Action>;
		using StatesContainer = std::set<states::State>;
		using RangesContainer = std::map<ranges::Range, math::Real>;

		class InputContext;
		/// <summary>
		/// A helper structure.
		/// </summary>
		struct MappedInput
		{
			ENGINE_API void ConsumeAction(actions::Action action) { actions.erase(action); }
			ENGINE_API void ConsumeState(states::State state) { states.erase(state); }
			ENGINE_API void ConsumeRange(ranges::Range range)
			{
				const auto rangeItr = ranges.find(range);
				if (rangeItr != ranges.end())
				{
					ranges.erase(rangeItr);
				}
			}

			ActionsContainer actions;
			StatesContainer states;
			RangesContainer ranges;
		}; /* end struct MappedInput */

		typedef void(*InputCallback)(MappedInput& mappedInputs);

		/// <summary>
		/// The input mapping. It maps the specific graphics library input (e.g. GLFW_KEY_W) into library-independent value (e.g. KEY_W).
		/// </summary>
		class InputMapping
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ENGINE_API InputMapping();
			ENGINE_API InputMapping(const std::string& configDirectory, const std::string& contextListFileName);
			ENGINE_API ~InputMapping();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Clears the actions collection.
			/// </summary>
			ENGINE_API void ClearActions();

			/// <summary>
			/// Clears the ranges collection.
			/// </summary>
			ENGINE_API void ClearRanges();

			ENGINE_API void SetRawButtonState(raw_input_keys::RawInputKey button, bool pressed, bool previouslyPressed);
			ENGINE_API void SetRawAxisValue(raw_input_axes::RawInputAxis axis, math::Real value);
			
			/// <summary>
			/// Input dispatching.
			/// </summary>
			ENGINE_API void Dispatch() const;
			
			/// <summary>
			/// Input callback registration. Add a callback to the dispatch table.
			/// </summary>
			ENGINE_API void RegisterCallback(InputCallback inputCallback, int priority);
			
			/// <summary>
			/// Context management interface. Pushes an active input context onto the stack.
			/// </summary>
			ENGINE_API void PushContext(const std::string& contextName);
			
			/// <summary>
			/// Context management interface. Pops the current input context off the stack.
			/// </summary>
			ENGINE_API void PopContext();
			ENGINE_API const MappedInput& GetMappedInput() const { return m_currentMappedInput; }
		private:
			actions::Action MapButtonToAction(raw_input_keys::RawInputKey button) const;
			states::State MapButtonToState(raw_input_keys::RawInputKey button) const;
			/// <summary>
			/// Helper method. It consumes all input mapped to a given button.
			/// </summary>
			void MapAndConsumeButton(raw_input_keys::RawInputKey button);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			std::map<std::string, std::unique_ptr<InputContext>> m_inputContexts;
			std::list<InputContext*> m_activeContexts;
			std::multimap<int, InputCallback> m_callbackTable;
			MappedInput m_currentMappedInput;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class InputMapping */
	
	} /* end namespace input */

} /* end namespace engine */

#endif // __ENGINE_INPUT_MAPPING_H__
