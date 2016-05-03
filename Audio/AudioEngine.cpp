#include "stdafx.h"
#include "AudioEngine.h"
#include "Utility\ILogger.h"

#include "fmod_errors.h" // for error-checking

Audio::AudioEngine::AudioEngine(int maxChannelsCount) :
	m_system(NULL),
	m_maxChannelsCount(maxChannelsCount)
{
	// Based on tutorial: https://cuboidzone.wordpress.com/category/tutorials/
	FMOD_RESULT fmodResult = FMOD::System_Create(&m_system); // Create the main system object
	CHECK_CONDITION_EXIT_ALWAYS(fmodResult == FMOD_OK, Utility::Critical, "Failed to create an audio system with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));

	int driversCount = 0;
	m_system->getNumDrivers(&driversCount);
	CHECK_CONDITION_EXIT_ALWAYS(driversCount != 0, Utility::Critical, "Failed to create an audio system. Drivers count = %d", driversCount);

	unsigned int version;
	fmodResult = m_system->getVersion(&version);
	CHECK_CONDITION_EXIT_ALWAYS(version >= FMOD_VERSION, Utility::Error,
		"Failed to create an audio system. FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

	fmodResult = m_system->init(m_maxChannelsCount, FMOD_INIT_NORMAL, NULL); // initialize FMOD
	CHECK_CONDITION_EXIT_ALWAYS(fmodResult == FMOD_OK, Utility::Critical, "Initializing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
	NOTICE_LOG("Audio engine created.");
}


Audio::AudioEngine::~AudioEngine()
{
	FMOD_RESULT fmodResult = FMOD_OK;
	for (Sounds::iterator soundItr = m_sounds.begin(); soundItr != m_sounds.end(); ++soundItr)
	{
		fmodResult = (*soundItr)->release();
		CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Releasing sound has ended with error code %d. %s",
			fmodResult, FMOD_ErrorString(fmodResult));
	}

	fmodResult = m_system->close();
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Closing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
	fmodResult = m_system->release();
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Releasing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
	NOTICE_LOG("Audio engine destroyed.");
}

void Audio::AudioEngine::Update(Math::Real deltaTime)
{
	FMOD_RESULT fmodResult = m_system->update();
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Updating audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
}

void Audio::AudioEngine::CreateSound(const std::string& soundFileName)
{
	FMOD::Sound* sound;
	FMOD_RESULT fmodResult = m_system->createSound(soundFileName.c_str(), FMOD_DEFAULT, 0, &sound);
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Creating sound \"%s\" has ended with error code %d. %s",
		soundFileName.c_str(), fmodResult, FMOD_ErrorString(fmodResult));
	if (fmodResult == FMOD_OK)
	{
		m_sounds.push_back(sound);
	}
}

void Audio::AudioEngine::Play()
{
	FMOD::Channel* channel = NULL;
	FMOD_RESULT fmodResult = m_system->playSound(m_sounds.front(), 0, false, &channel);
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Playing sound has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
}