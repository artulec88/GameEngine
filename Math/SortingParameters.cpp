#include "stdafx.h"
#include "SortingParameters.h"
#include "Utility/ILogger.h"

math::sorting::SortingParametersChain::SortingParametersChain(keys::Key sortingKey, orders::Order sortingOrder) :
	m_sortingParametersChain(nullptr),
	m_key(sortingKey),
	m_order(sortingOrder)
{
}

math::sorting::SortingParametersChain::~SortingParametersChain()
{
}

math::sorting::SortingParametersChain::SortingParametersChain(const SortingParametersChain& sortingParameters) :
	m_sortingParametersChain(sortingParameters.m_sortingParametersChain),
	m_key(sortingParameters.m_key),
	m_order(sortingParameters.m_order)
{
}

void math::sorting::SortingParametersChain::ResetChainLink()
{
	m_sortingParametersChain = nullptr;
}

void math::sorting::SortingParametersChain::SetKey(keys::Key sortingKey)
{
	if (m_sortingParametersChain != nullptr && m_sortingParametersChain->GetKey() == sortingKey)
	{
		WARNING_LOG_MATH("Sorting parameter's child specify the same sorting key so it will be removed");
		m_sortingParametersChain = nullptr;
	}
	m_key = sortingKey;
}

void math::sorting::SortingParametersChain::AddChainLink(SortingParametersChain* sortingParametersChain)
{
	if (m_sortingParametersChain != nullptr)
	{
		INFO_LOG_MATH("Replacing the already set-up sorting parameters chain");
		m_sortingParametersChain = nullptr;
	}
	if (m_key == sortingParametersChain->GetKey())
	{
		ERROR_LOG_MATH("Cannot add sorting parameters chain link when both parent and child link specify the same sorting key");
		return;
	}
	m_sortingParametersChain = sortingParametersChain;
}