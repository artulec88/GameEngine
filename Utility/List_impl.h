#include "stdafx.h"
#ifndef __UTILITY_CONTAINER_LIST_IMPL_H__
#define __UTILITY_CONTAINER_LIST_IMPL_H__

#include "List.h"
#include "ILogger.h"

template <class T>
Utility::Container::List<T>::List(const T& item, List<T>* tailList = nullptr) :
	m_item(item),
	m_tailList(tailList)
{
}

template <class T>
Utility::Container::List<T>::~List()
{
}

template <class T>
void Utility::Container::List<T>::SetItem(const T& item)
{
	m_item = item;
}

template <class T>
T& Utility::Container::List<T>::GetItem()
{
	return m_item;
}

template <class T>
const T& Utility::Container::List<T>::GetItem() const
{
	return m_item;
}

template <class T>
void Utility::Container::List<T>::SetTail(List<T>* tailList)
{
	m_tailList = tailList;
}

template <class T>
Utility::Container::List<T>* Utility::Container::List<T>::GetTail()
{
	return m_tailList;
}

template <class T>
const Utility::Container::List<T>* Utility::Container::List<T>::GetTail() const
{
	return m_tailList;
}

template <class T>
int Utility::Container::List<T>::GetSize() const
{
	int size = 1;
	while (m_tailList != nullptr)
	{
		m_tailList = m_tailList->GetTail();
		++size;
	}
	return size;
}

template <class T>
void Utility::Container::List<T>::RemoveTailFront()
{
	if (m_tailList == nullptr)
	{
		return;
	}
	List<T>* tailFront = m_tailList;
	m_tailList = m_tailList->GetTail();
	SAFE_DELETE(tailFront);
}

template <class T>
void Utility::Container::List<T>::RemoveTailItem(const T& itemToRemove, bool removeAllMatchingNodesEnabled)
{
	List<T>* prevNode = this;
	List<T>* tailList = m_tailList;
	while (tailList != nullptr)
	{
		if (tailList->GetItem() == itemToRemove)
		{
			tailList = tailList->GetTail();
			prevNode->RemoveTailFront();
			if (!removeAllMatchingNodesEnabled)
			{
				break;
			}
		}
		else
		{
			prevNode = tailList;
			tailList = tailList->GetTail();
		}
	}
}

#endif /* __UTILITY_CONTAINER_LIST_IMPL_H__ */