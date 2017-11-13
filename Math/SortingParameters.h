#ifndef __MATH_SORTING_PARAMETERS_H__
#define __MATH_SORTING_PARAMETERS_H__

#include "Math.h"

namespace Math
{
	namespace Sorting
	{
		namespace Keys
		{
			/// <summary> The sorting key represents the attribute based on which the ordering is performed. </summary>
			enum Key
			{
				VALUE = 0,
				COMPONENT_X,
				COMPONENT_Y,
				COMPONENT_Z,
				SUM_OF_COMPONENTS,
				SUM_OF_ABSOLUTE_COMPONENTS,
				SUM_OF_SQUARED_COMPONENTS
			}; /* end enum SortingKey */
		} /* end namespace SortingKeys */

		namespace Orders
		{
			/// <summary> The direction represents the order in which the sorting is performed. </summary>
			enum Order
			{
				ASCENDING = 0,
				DESCENDING
			}; /* end enum Order */
		} /* end namespace Orders */

		class SortingParametersChain
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API SortingParametersChain(Keys::Key sortingKey, Orders::Order sortingOrder);
			MATH_API ~SortingParametersChain();
			SortingParametersChain(const SortingParametersChain& sortingParametersChain) = delete;
			SortingParametersChain(SortingParametersChain&& sortingParametersChain) = delete;
			SortingParametersChain& operator=(const SortingParametersChain& sortingParametersChain) = delete;
			SortingParametersChain& operator=(SortingParametersChain&& sortingParametersChain) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			MATH_API void ResetChainLink();
			MATH_API void AddChainLink(SortingParametersChain* sortingChainParameters);
			MATH_API Keys::Key GetKey() const { return m_sortingKey; }
			MATH_API Orders::Order GetOrder() const { return m_order; }
			MATH_API void SetKey(Keys::Key sortingKey);
			MATH_API void SetOrder(Orders::Order sortingOrder) { m_order = sortingOrder; }
			MATH_API const SortingParametersChain* GetSortingParametersChain() const { return m_sortingParametersChain; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			SortingParametersChain* m_sortingParametersChain;
			Keys::Key m_sortingKey;
			Orders::Order m_order;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SortingParametersChain */

	} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_SORTING_PARAMETERS_H__ */