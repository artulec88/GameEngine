#ifndef __AUDIO_SOUND_H__
#define __AUDIO_SOUND_H__

#include "Audio.h"

#include "fmod.hpp"
#include <string>

namespace audio
{
	class SoundData
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SoundData(const std::string& audioFileName);
		~SoundData();
		SoundData(const SoundData& soundData) = delete;
		SoundData(SoundData&& soundData) = delete;
		SoundData& operator=(const SoundData& soundData) = delete;
		SoundData& operator=(SoundData&& soundData) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		FMOD::Sound* GetSoundData() const { return m_soundData; }
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
		AUDIO_API ~Sound();

		/// <summary> Sound copy constructor. </summary>
		/// <param name="sound"> The sound to copy construct from. </param>
		Sound(const Sound& sound) = delete;

		/// <summary> Sound move constructor. </summary>
		/// <param name="sound"> The sound to move construct from. </param>
		Sound(Sound&& sound) = delete;

		/// <summary> Sound copy assignment operator. </summary>
		/// <param name="sound"> The sound to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned sound. </returns>
		Sound& operator=(const Sound& sound) = delete;

		/// <summary> Sound move assignment operator. </summary>
		/// <param name="sound"> The sound to move assign from. </param>
		/// <returns> The reference to the newly move-assigned sound. </returns>
		Sound& operator=(Sound&& sound) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		//AUDIO_API void Update(math::Real deltaTime);
		//AUDIO_API void CreateSound(const std::string& soundFileName);
		//AUDIO_API void Play(bool inLoop = false);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
		const std::string m_audioFileName;
		SoundData* m_soundData;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Sound */

} /* end namespace audio */

#endif /* __AUDIO_SOUND_H__ */
