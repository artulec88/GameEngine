#include "stdafx.h"
#include "FogInfo.h"

Rendering::FogEffect::FogInfo::FogInfo(Color fogColor /* = Color(ColorNames::GAINSBORO) */, Math::Real fogStartDistance /* = 8.0f */, Math::Real fogEndDistance /* = 50.0f */, Math::Real fogDensityFactor /* = 0.2f */,
	Math::Real fogGradient /* = 0.005f */, FogFallOffType fogFallOffType /* = EXPONENTIAL */, FogCalculationType fogCalculationType /* = RANGE_BASED */, bool isEnabled /* = true */) :
	m_color(fogColor),
	m_startDistance(fogStartDistance),
	m_endDistance(fogEndDistance),
	m_densityFactor(fogDensityFactor),
	m_gradient(fogGradient),
	m_fallOffType(fogFallOffType),
	m_calculationType(fogCalculationType),
	m_isEnabled(isEnabled)
{
}

Rendering::FogEffect::FogInfo::~FogInfo()
{
}

Rendering::FogEffect::FogInfo::FogInfo(const FogInfo& fogInfo) :
	m_color(fogInfo.m_color),
	m_startDistance(fogInfo.m_startDistance),
	m_endDistance(fogInfo.m_endDistance),
	m_densityFactor(fogInfo.m_densityFactor),
	m_gradient(fogInfo.m_gradient),
	m_fallOffType(fogInfo.m_fallOffType),
	m_calculationType(fogInfo.m_calculationType),
	m_isEnabled(fogInfo.m_isEnabled)
{
}

Rendering::FogEffect::FogInfo::FogInfo(FogInfo&& fogInfo) :
	m_color(std::move(fogInfo.m_color)),
	m_startDistance(std::move(fogInfo.m_startDistance)),
	m_endDistance(std::move(fogInfo.m_endDistance)),
	m_densityFactor(std::move(fogInfo.m_densityFactor)),
	m_gradient(std::move(fogInfo.m_gradient)),
	m_fallOffType(std::move(fogInfo.m_fallOffType)),
	m_calculationType(std::move(fogInfo.m_calculationType)),
	m_isEnabled(std::move(fogInfo.m_isEnabled))
{
}

void Rendering::FogEffect::FogInfo::operator=(const FogInfo& fogInfo)
{
	m_color = fogInfo.m_color;
	m_startDistance = fogInfo.m_startDistance;
	m_endDistance = fogInfo.m_endDistance;
	m_densityFactor = fogInfo.m_densityFactor;
	m_gradient = fogInfo.m_gradient;
	m_fallOffType = fogInfo.m_fallOffType;
	m_calculationType = fogInfo.m_calculationType;
	m_isEnabled = fogInfo.m_isEnabled;
}

void Rendering::FogEffect::FogInfo::operator=(FogInfo&& fogInfo)
{
	m_color = std::move(fogInfo.m_color);
	m_startDistance = std::move(fogInfo.m_startDistance);
	m_endDistance = std::move(fogInfo.m_endDistance);
	m_densityFactor = std::move(fogInfo.m_densityFactor);
	m_gradient = std::move(fogInfo.m_gradient);
	m_fallOffType = std::move(fogInfo.m_fallOffType);
	m_calculationType = std::move(fogInfo.m_calculationType);
	m_isEnabled = std::move(fogInfo.m_isEnabled);
}

bool Rendering::FogEffect::FogInfo::operator<(const FogInfo& fogInfo) const
{
	if (m_fallOffType != fogInfo.m_fallOffType)
	{
		return m_fallOffType < fogInfo.m_fallOffType;
	}
	else
	{
		return m_calculationType < fogInfo.m_calculationType;
	}
	//return (fallOffType & calculationType) < (fog.fallOffType & fog.calculationType);
}