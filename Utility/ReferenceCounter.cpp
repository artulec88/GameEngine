#include "stdafx.h"
#include "ReferenceCounter.h"

Utility::ReferenceCounter::ReferenceCounter() :
	m_refCount(1)
{
}

Utility::ReferenceCounter::~ReferenceCounter()
{
}

void Utility::ReferenceCounter::AddReference()
{
	++m_refCount;
}

void Utility::ReferenceCounter::RemoveReference()
{
	--m_refCount;
}

bool Utility::ReferenceCounter::IsReferenced() const
{
	return m_refCount > 0;
}