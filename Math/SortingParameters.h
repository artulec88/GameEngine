#ifndef __MATH_SORTING_PARAMETERS_H__
#define __MATH_SORTING_PARAMETERS_H__

#include "Math.h"

namespace Math
{
	namespace Sorting
	{

		/// <summary> The sorting key represents the attribute based on which the ordering is performed. </summary>
		enum MATH_API SortingKey
		{
			VALUE = 0,
			COMPONENT_X,
			COMPONENT_Y,
			COMPONENT_Z,
			SUM_OF_COMPONENTS,
			SUM_OF_ABSOLUTE_COMPONENTS,
			SUM_OF_SQUARED_COMPONENTS
		};

		/// <summary> The direction represents the order in which the sorting is performed. </summary>
		enum MATH_API SortingDirection
		{
			ASCENDING = 0,
			DESCENDING
		};

		class MATH_API SortingParametersChain
		{
		public:
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			SortingParametersChain(SortingKey sortingKey, SortingDirection sortingDirection);
			virtual ~SortingParametersChain(void);
			SortingParametersChain(const SortingParametersChain& sortingParametersChain) = delete;
			SortingParametersChain(SortingParametersChain&& sortingParametersChain) = delete;
			SortingParametersChain& operator=(const SortingParametersChain& sortingParametersChain) = delete;
			SortingParametersChain& operator=(SortingParametersChain&& sortingParametersChain) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			void ResetChainLink();
			void AddChainLink(SortingParametersChain* sortingChainParameters);
			SortingKey GetSortingKey() const { return m_sortingKey; }
			SortingDirection GetSortingDirection() const { return m_sortingDirection; }
			void SetSortingKey(SortingKey sortingKey);
			void SetSortingDirection(SortingDirection sortingDirection) { m_sortingDirection = sortingDirection; }
			const SortingParametersChain* GetSortingParametersChain() const { return m_sortingParametersChain; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			SortingParametersChain* m_sortingParametersChain;
			SortingKey m_sortingKey;
			SortingDirection m_sortingDirection;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SortingParametersChain */

	} /* end namespace Sorting */

} /* end namespace Math */

#endif /* __MATH_SORTING_PARAMETERS_H__ */