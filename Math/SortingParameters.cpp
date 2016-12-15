#include "stdafx.h"
#include "SortingParameters.h"
#include "Utility\ILogger.h"

Math::Sorting::SortingParametersChain::SortingParametersChain(Keys::Key sortingKey, Orders::Order sortingOrder) :
	m_sortingParametersChain(nullptr),
	m_sortingKey(sortingKey),
	m_order(sortingOrder)
{
}

Math::Sorting::SortingParametersChain::~SortingParametersChain(void)
{
}

void Math::Sorting::SortingParametersChain::ResetChainLink()
{
	m_sortingParametersChain = nullptr;
}

void Math::Sorting::SortingParametersChain::SetKey(Keys::Key sortingKey)
{
	if ((m_sortingParametersChain != nullptr) && (m_sortingParametersChain->GetKey() == sortingKey))
	{
		WARNING_LOG_MATH("Sorting parameter's child specify the same sorting key so it will be removed");
		m_sortingParametersChain = nullptr;
	}
	m_sortingKey = sortingKey;
}

void Math::Sorting::SortingParametersChain::AddChainLink(SortingParametersChain* sortingParametersChain)
{
	if (m_sortingParametersChain != nullptr)
	{
		INFO_LOG_MATH("Replacing the already set-up sorting parameters chain");
		m_sortingParametersChain = nullptr;
	}
	if (m_sortingKey == sortingParametersChain->GetKey())
	{
		ERROR_LOG_MATH("Cannot add sorting parameters chain link when both parent and child link specify the same sorting key");
		return;
	}
	m_sortingParametersChain = sortingParametersChain;
}