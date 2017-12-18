#include "stdafx.h"
#include "Sound.h"
#include "Utility/ILogger.h"

audio::SoundData::SoundData(const std::string& audioFileName) :
	m_soundData(nullptr)
{

}

audio::SoundData::~SoundData()
{
	m_soundData->release();
}

audio::Sound::Sound(const std::string& audioFileName, FMOD_MODE mode) :
	m_audioFileName(audioFileName),
	m_soundData(nullptr)
{
}

audio::Sound::~Sound()
{
	CHECK_CONDITION_AUDIO(m_soundData != nullptr, utility::logging::WARNING, "Destructing the sound \"", m_audioFileName, "\" aborted. Sound data is already NULL.");
	//m_soundData->RemoveReference();
	//if (!m_soundData->IsReferenced())
	{
		if (m_audioFileName.length() > 0)
		{
			//meshResourceMap.erase(m_audioFileName);
		}
		SAFE_DELETE(m_soundData);
	}
}
