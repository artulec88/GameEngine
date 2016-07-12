#include "stdafx.h"
#include "Array.h"
#include "ILogger.h"

template <class T>
Utility::Container::Array<T>::Array(int initialSize /* = 1 */, int growSizeByValue /* = 1 */) :
	m_data(NULL),
	m_size(initialSize),
	m_maxSize(initialSize),
	m_growSizeByValue(growSizeByValue)
{
	m_data = new T[initialSize];
}

template <class T>
Utility::Container::Array<T>::~Array()
{
	SAFE_DELETE_JUST_TABLE(m_data);
}

template <class T>
int Utility::Container::Array<T>::GetSize() const
{
	return m_size;
}

template <class T>
T* Utility::Container::Array<T>::GetArray()
{
	return m_data;
}

template <class T>
const T* Utility::Container::Array<T>::GetArray() const
{
	return m_data;
}

template <class T>
T& Utility::Container::Array<T>::operator[](int i)
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data[0], Logging::ERR, "The array [] operator cannot access index %d which is outside of range [0; %d)", i, m_size);
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < m_size, m_data[m_size - 1], Logging::ERR, "The array [] operator cannot access index %d which is outside of range [0; %d)", i, m_size);
	return m_data[i];
}

template <class T>
const T& Utility::Container::Array<T>::operator[](int i) const
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i >= 0, m_data[0], Logging::ERR, "The array [] operator cannot access index %d which is outside of range [0; %d)", i, m_size);
	CHECK_CONDITION_RETURN_ALWAYS_UTILITY(i < m_size, m_data[m_size - 1], Logging::ERR, "The array [] operator cannot access index %d which is outside of range [0; %d)", i, m_size);
	return m_data[i];
}

template <class T>
void Utility::Container::Array<T>::Append(const T& dataElement)
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	if (m_size == m_maxSize)
	{
		WARNING_LOG_UTILITY("The array cannot hold more than %d elements", m_maxSize);
	}
	m_data[++m_size] = dataElement;
}

template <class T>
void Utility::Container::Array<T>::SetElement(int i, const T& dataElement)
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i >= 0, Logging::ERR, "Cannot set array's element at index %d. The correct index must be in the range [0; %d)", i, m_size);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i < m_maxSize, Logging::WARNING, "The array cannot hold more than %d elements", m_maxSize);

	m_data[i] = dataElement;
	if (i > m_size)
	{
		m_size = i;
	}
}

template<class T>
void Utility::Container::Array<T>::Remove(int i)
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i >= 0, Logging::ERR, "Cannot remove array's element at index %d. The correct index must be in the range [0; %d)", i, m_size);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(i < m_size, Logging::WARNING, "Cannot remove array's element at index %d. The correct index must be in the range [0; %d)", i, m_size);

	for (int index = i + 1; index < m_size; ++index)
	{
		m_data[index - 1] = m_data[index];
	}
	--m_size;
}

template<class T>
void Utility::Container::Array<T>::RemoveAll()
{
	WARNING_LOG_UTILITY("This function has not been tested yet");
	for (int index = 0; index < m_size; ++index)
	{
		m_data[index] = T();
	}
	m_size = 0;
}

template<class T>
void Utility::Container::Array<T>::SetMaxSize(int maxSize, bool copy)
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

template<class T>
int Utility::Container::Array<T>::GetMaxSize() const
{
	return m_maxSize;
}

template<class T>
void Utility::Container::Array<T>::SetGrowSizeByValue(int growSizeByValue)
{
	m_growSizeByValue = growSizeByValue;
}

template<class T>
int Utility::Container::Array<T>::GetGrowSizeByValue() const
{
	return m_growSizeByValue;
}
