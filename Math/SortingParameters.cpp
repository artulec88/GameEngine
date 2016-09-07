#include "stdafx.h"
#include "SortingParameters.h"
#include "Utility\ILogger.h"

Math::Sorting::SortingParametersChain::SortingParametersChain(SortingKey sortingKey, SortingDirection sortingDirection) :
	m_sortingParametersChain(NULL),
	m_sortingKey(sortingKey),
	m_sortingDirection(sortingDirection)
{
}

Math::Sorting::SortingParametersChain::~SortingParametersChain(void)
{
	SAFE_DELETE(m_sortingParametersChain);
}

void Math::Sorting::SortingParametersChain::ResetChainLink()
{
	SAFE_DELETE(m_sortingParametersChain);
}

void Math::Sorting::SortingParametersChain::SetSortingKey(SortingKey sortingKey)
{
	if ((m_sortingParametersChain != NULL) && (m_sortingParametersChain->GetSortingKey() == sortingKey))
	{
		WARNING_LOG_MATH("Sorting parameter's child specify the same sorting key so it will be removed");
		SAFE_DELETE(m_sortingParametersChain);
	}
	m_sortingKey = sortingKey;
}

void Math::Sorting::SortingParametersChain::AddChainLink(SortingParametersChain* sortingParametersChain)
{
	if (m_sortingParametersChain != NULL)
	{
		INFO_LOG_MATH("Replacing the already set-up sorting parameters chain");
		SAFE_DELETE(m_sortingParametersChain);
	}
	if (m_sortingKey == sortingParametersChain->GetSortingKey())
	{
		ERROR_LOG_MATH("Cannot add sorting parameters chain link when both parent and child link specify the same sorting key");
		return;
	}
	m_sortingParametersChain = sortingParametersChain;
}