#include "stdafx.h"
#include "Sound.h"
#include "Utility\ILogger.h"

Audio::SoundData::SoundData(const std::string& audioFileName) :
	m_soundData(NULL)
{

}

Audio::SoundData::~SoundData()
{
	m_soundData->release();
}

Audio::Sound::Sound(const std::string& audioFileName, FMOD_MODE mode) :
	m_audioFileName(audioFileName),
	m_soundData(NULL)
{
}

Audio::Sound::~Sound(void)
{
	ASSERT(m_soundData != NULL);
	if (m_soundData == NULL)
	{
		WARNING_LOG("Destructing the sound aborted. Sound data is already NULL.");
	}
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