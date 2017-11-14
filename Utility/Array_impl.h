#include "stdafx.h"
#ifndef __UTILITY_CONTAINER_ARRAY_IMPL_H__
#define __UTILITY_CONTAINER_ARRAY_IMPL_H__

#include "Array.h"
#include "ILogger.h"

template <class T, unsigned int Size>
T& utility::container::Array<T, Size>::operator[](const unsigned int i)
{
	//CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data.front(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");
	//CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < GetSize(), m_data.back(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");

	CHECK_CONDITION_UTILITY(i >= 0 && i < GetSize(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");
	return m_data[i];
}

template <class T, unsigned int Size>
const T& utility::container::Array<T, Size>::operator[](const unsigned int i) const
{
	//CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data.front(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");
	//CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < GetSize(), m_data.back(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");

	CHECK_CONDITION_UTILITY(i >= 0 && i < GetSize(), Logging::ERR, "The array [] operator cannot access index ", i, " which is outside of range [0; ", GetSize(), ")");
	return m_data[i];
}

template <class T, unsigned int Size>
void utility::container::Array<T, Size>::SetElement(unsigned int i, const T& dataElement)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i >= 0, logging::ERR, "Cannot set array's element at index ", i, ". The correct index must be in the range [0; ", GetSize(), ")");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i < GetSize(), logging::ERR, "The array cannot hold more than ", GetSize(), " elements");

	m_data[i] = dataElement;
}

#endif /* __UTILITY_CONTAINER_ARRAY_IMPL_H__ */