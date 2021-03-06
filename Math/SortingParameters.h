#ifndef __MATH_SORTING_PARAMETERS_H__
#define __MATH_SORTING_PARAMETERS_H__

#include "Math.h"

namespace math
{
	namespace sorting
	{
		namespace keys
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
		} /* end namespace keys */

		namespace orders
		{
			/// <summary> The direction represents the order in which the sorting is performed. </summary>
			enum Order
			{
				ASCENDING = 0,
				DESCENDING
			}; /* end enum Order */
		} /* end namespace orders */

		class SortingParametersChain
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Sorting parameters chain constructor. </summary>
			/// <param name="sortingKey"> The key by which the sorting will be performed. </param>
			/// <param name="sortingOrder"> The order of sorting. </param>
			MATH_API SortingParametersChain(keys::Key sortingKey, orders::Order sortingOrder);

			/// <summary> Sorting parameters chain destructor. </summary>
			MATH_API ~SortingParametersChain();

			/// <summary> Sorting parameters chain copy constructor. </summary>
			/// <param name="sortingParametersChain"> The sorting parameters chain to copy construct from. </param>
			SortingParametersChain(const SortingParametersChain& sortingParametersChain);

			SortingParametersChain(SortingParametersChain&& sortingParametersChain) = delete;
			SortingParametersChain& operator=(const SortingParametersChain& sortingParametersChain) = delete;
			SortingParametersChain& operator=(SortingParametersChain&& sortingParametersChain) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			MATH_API void ResetChainLink();
			MATH_API void AddChainLink(SortingParametersChain* sortingChainParameters);
			MATH_API keys::Key GetKey() const { return m_key; }
			MATH_API orders::Order GetOrder() const { return m_order; }
			MATH_API void SetKey(keys::Key sortingKey);
			MATH_API void SetOrder(const orders::Order sortingOrder) { m_order = sortingOrder; }
			MATH_API const SortingParametersChain* GetSortingParametersChain() const { return m_sortingParametersChain; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			SortingParametersChain* m_sortingParametersChain;
			keys::Key m_key;
			orders::Order m_order;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SortingParametersChain */

	} /* end namespace sorting */

} /* end namespace math */

#endif /* __MATH_SORTING_PARAMETERS_H__ */