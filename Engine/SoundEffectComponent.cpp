#include "stdafx.h"
#include "SoundEffectComponent.h"
#include "CoreEngine.h"

Engine::SoundEffectComponent::SoundEffectComponent(const std::string& soundEffectPath) :
	m_soundEffectPath(soundEffectPath)
{
	//CoreEngine::GetCoreEngine()->GetAudioEngine()->LoadSoundEffect3D(m_soundEffectPath);
}


Engine::SoundEffectComponent::~SoundEffectComponent()
{
}
