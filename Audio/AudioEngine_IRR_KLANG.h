#ifndef __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__
#define __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__

#include "Audio.h"
#include "IAudioEngine.h"
#include "irrKlang.h"

namespace Audio
{
	class AudioEngine_IRR_KLANG : public IAudioEngine
	{
		/* TODO: Better key for more efficient map search ? */
		using Filenames2SoundSources = std::map<std::string, irrklang::ISoundSource*>;

		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit AudioEngine_IRR_KLANG(const std::string& audioDirectory, int maxChannelsCount);
		AUDIO_API ~AudioEngine_IRR_KLANG(void);
	private:
		AudioEngine_IRR_KLANG(const AudioEngine_IRR_KLANG& audioEngine);
		void operator=(const AudioEngine_IRR_KLANG& audioEngine);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API virtual void Update(Math::Real deltaTime);
		AUDIO_API virtual void LoadSoundEffect(const std::string& path);
		AUDIO_API virtual void LoadSoundEffect3D(const std::string& path);
		AUDIO_API virtual void LoadSong(const std::string& path);

		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */);
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch);
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch, const Math::Vector3D& position, const Math::Vector3D& velocity);
		AUDIO_API virtual void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */);
		AUDIO_API virtual void StopSoundEffects();
		AUDIO_API virtual void StopSong();
		AUDIO_API virtual void SetMasterVolume(Math::Real volume);
		AUDIO_API virtual void SetSoundEffectsVolume(Math::Real volume);
		AUDIO_API virtual void SetSongsVolume(Math::Real volume);
	private:
		void Load(Categories::Category type, const std::string& path);

		/// <summary>
		/// A helper method for calculating the octaves. By using this method it becomes simple to modify the pitch of a sound.
		/// For example, to decrease the pitch of a sound by 3 octaves, we could do the following:
		/// <code>
		/// float frequency;
		/// channel->getFrequency(&frequency);
		/// float newFrequency = ChangeOctave(frequency, -3.0f);
		/// channel->setFrequency(newFrequency);
		/// </code>
		/// Every time we multiply / divide a frequency by two we get a new frequency that sounds one octave higher / lower.
		/// </summary>
		/// <returns>
		/// New frequency of a sound.
		/// </returns>
		virtual Math::Real ChangeOctave(Math::Real frequency, Math::Real variation) const;

		/// <summary>
		/// A helper method for calculating the semitones. By using this method it becomes simple to modify the pitch of a sound.
		/// For example, to decrease the pitch of a sound by 3 semitones, we could do the following:
		/// <code>
		/// float frequency;
		/// channel->getFrequency(&frequency);
		/// float newFrequency = ChangeSemitone(frequency, -3.0f);
		/// channel->setFrequency(newFrequency);
		/// </code>
		/// Every time we multiply / divide a frequency by two and a half we get a new frequency that sounds one semitone higher / lower.
		/// </summary>
		/// <returns>
		/// New frequency of a sound.
		/// </returns>
		virtual Math::Real ChangeSemitone(Math::Real frequency, Math::Real variation) const;

		void SetVolume(Categories::Category type, Math::Real volume);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const float M_SONG_FADE_IN_TIME;
		irrklang::ISoundEngine* m_engine;
		Filenames2SoundSources m_soundSources[Categories::COUNT];

		irrklang::ISound* m_currentSong;
		std::string m_currentSongPath;
		std::string m_nextSongPath;
		
		FadeStates::FadeState m_fade;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine_IRR_KLANG */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__ */
