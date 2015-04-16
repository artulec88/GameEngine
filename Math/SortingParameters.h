#pragma once

#include "Math.h"

namespace Math { namespace Sorting
{

enum MATH_API SortingKey { VALUE = 0, COMPONENT_X, COMPONENT_Y, COMPONENT_Z, SUM_OF_COMPONENTS, SUM_OF_ABSOLUTE_COMPONENTS, SUM_OF_SQUARED_COMPONENTS };
enum MATH_API SortingDirection { ASCENDING = 0, DESCENDING };

class SortingParametersChain
{
public:
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API SortingParametersChain(SortingKey sortingKey, SortingDirection sortingDirection);
	MATH_API virtual ~SortingParametersChain(void);
private:
	SortingParametersChain(const SortingParametersChain& sortingParametersChain) {} // don't implement
	void operator=(const SortingParametersChain& sortingParametersChain) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API void ResetChainLink();
	MATH_API void AddChainLink(SortingParametersChain* sortingChainParameters);
	MATH_API SortingKey GetSortingKey() const { return m_sortingKey; }
	MATH_API SortingDirection GetSortingDirection() const { return m_sortingDirection; }
	MATH_API void SetSortingKey(SortingKey sortingKey);
	MATH_API void SetSortingDirection(SortingDirection sortingDirection) { m_sortingDirection = sortingDirection; }
	MATH_API const SortingParametersChain* GetSortingParametersChain() const { return m_sortingParametersChain; }
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
