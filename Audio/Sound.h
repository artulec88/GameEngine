#ifndef __AUDIO_SOUND_H__
#define __AUDIO_SOUND_H__

#include "Audio.h"
#include "Utility\ReferenceCounter.h"
#include <string>

namespace Audio
{
	class SoundData : Utility::ReferenceCounter
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SoundData(const std::string& audioFileName);
		~SoundData(void);
	private:
		SoundData(const SoundData& soundData);
		void operator=(const SoundData& soundData);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		FMOD::Sound* GetSoundData() { return m_soundData; }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		FMOD::Sound* m_soundData;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class SoundData */

	class Sound
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit Sound(const std::string& audioFileName, FMOD_MODE mode);
		AUDIO_API ~Sound(void);
	private:
		Sound(const Sound& sound);
		void operator=(const Sound& sound);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//AUDIO_API void Update(Math::Real deltaTime);
		//AUDIO_API void CreateSound(const std::string& soundFileName);
		//AUDIO_API void Play(bool inLoop = false);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const std::string m_audioFileName;
		SoundData* m_soundData;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Sound */

} /* end namespace Audio */

#endif /* __AUDIO_SOUND_H__ */
