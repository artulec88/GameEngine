#include "stdafx.h"

#ifndef __UTILITY_CONTAINER_VECTOR_IMPL_H__
#define __UTILITY_CONTAINER_VECTOR_IMPL_H__

#include "Vector.h"
#include "ILogger.h"

template <class T>
T& Utility::Container::Vector<T>::operator[](unsigned int i)
{
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data[0], Logging::ERR, "The vector [] operator cannot access index ", i, " which is outside of range [0; ", m_size, ")");
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < m_size, m_data[m_size - 1], Logging::ERR, "The vector [] operator cannot access index ", i, " which is outside of range [0; ", m_size, ")");
	return m_data[i];
}

template <class T>
const T& Utility::Container::Vector<T>::operator[](unsigned int i) const
{
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data[0], Logging::ERR, "The vector [] operator cannot access index ", i, " which is outside of range [0; ", m_size, ")");
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < m_size, m_data[m_size - 1], Logging::ERR, "The vector [] operator cannot access index ", i, " which is outside of range [0; ", m_size, ")");
	return m_data[i];
}

template <class T>
void Utility::Container::Vector<T>::Append(const T& dataElement)
{
	if (m_size == m_maxSize)
	{
		WARNING_LOG_UTILITY("The vector cannot hold more than ", m_maxSize, " elements");
	}
	m_data[++m_size] = dataElement;
}

template <class T>
void Utility::Container::Vector<T>::SetElement(unsigned int i, const T& dataElement)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i >= 0, Logging::ERR, "Cannot set vector's element at index ", i, ". The correct index must be in the range [0; ", m_size, ")");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i < m_maxSize, Logging::WARNING, "The vector cannot hold more than ", m_maxSize, " elements");

	m_data[i] = dataElement;
	if (i > m_size)
	{
		m_size = i;
	}
}

template<class T>
void Utility::Container::Vector<T>::Remove(unsigned int i)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i >= 0, Logging::ERR, "Cannot remove vector's element at index ", i, ". The correct index must be in the range [0; ", m_size, ")");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i < m_size, Logging::WARNING, "Cannot remove vector's element at index ", i, ". The correct index must be in the range [0; ", m_size, ")");

	for (auto index = i + 1; index < m_size; ++index)
	{
		m_data[index - 1] = m_data[index];
	}
	--m_size;
}

template<class T>
void Utility::Container::Vector<T>::RemoveAll()
{
	for (unsigned int index = 0; index < m_size; ++index)
	{
		m_data[index] = T();
	}
	m_size = 0;
}

template<class T>
void Utility::Container::Vector<T>::SetMaxSize(unsigned int maxSize, bool copy)
{
	CRITICAL_LOG_UTILITY("This function is not yet implemented");
	//if (maxSize == 0)
	//{
	//	RemoveAll();
	//}
	//else if (maxSize == m_maxSize)
	//{
	//	return;
	//}
	//else if ()
	m_maxSize = maxSize;
}

#endif /* __UTILITY_CONTAINER_VECTOR_IMPL_H__ */