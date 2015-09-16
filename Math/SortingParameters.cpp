#include "stdafx.h"
#include "SortingParameters.h"
#include "Utility\ILogger.h"

using namespace Math::Sorting;

SortingParametersChain::SortingParametersChain(SortingKey sortingKey, SortingDirection sortingDirection) :
	m_sortingParametersChain(NULL),
	m_sortingKey(sortingKey),
	m_sortingDirection(sortingDirection)
{
}

SortingParametersChain::~SortingParametersChain(void)
{
	SAFE_DELETE(m_sortingParametersChain);
}

void SortingParametersChain::ResetChainLink()
{
	SAFE_DELETE(m_sortingParametersChain);
}

void SortingParametersChain::SetSortingKey(SortingKey sortingKey)
{
	if ((m_sortingParametersChain != NULL) && (m_sortingParametersChain->GetSortingKey() == sortingKey))
	{
		WARNING_LOG("Sorting parameter's child specify the same sorting key so it will be removed");
		SAFE_DELETE(m_sortingParametersChain);
	}
	m_sortingKey = sortingKey;
}

void SortingParametersChain::AddChainLink(SortingParametersChain* sortingParametersChain)
{
	if (m_sortingParametersChain != NULL)
	{
		INFO_LOG("Replacing the already set-up sorting parameters chain");
		SAFE_DELETE(m_sortingParametersChain);
	}
	if (m_sortingKey == sortingParametersChain->GetSortingKey())
	{
		ERROR_LOG("Cannot add sorting parameters chain link when both parent and child link specify the same sorting key");
		return;
	}
	m_sortingParametersChain = sortingParametersChain;
}