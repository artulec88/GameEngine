#ifndef __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__
#define __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__

#include "Audio.h"
#include "IAudioEngine.h"
#include "irrKlang.h"

#include <map>

namespace audio
{
	class AudioEngineIrrKlang : public IAudioEngine
	{
		/* TODO: Better key for more efficient map search ? */
		using Filenames2SoundSources = std::map<std::string, irrklang::ISoundSource*>;

		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit AudioEngineIrrKlang(const std::string& audioDirectory, int maxChannelsCount);
		AUDIO_API ~AudioEngineIrrKlang();

		/// <summary> irrKlang audio engine copy constructor. </summary>
		/// <param name="audioEngine"> The irrKlang audio engine to copy construct from. </param>
		AudioEngineIrrKlang(const AudioEngineIrrKlang& audioEngine) = delete;

		/// <summary> irrKlang audio engine move constructor. </summary>
		/// <param name="audioEngine"> The irrKlang audio engine to move construct from. </param>
		AudioEngineIrrKlang(AudioEngineIrrKlang&& audioEngine) = delete;

		/// <summary> irrKlang audio engine copy assignment operator. </summary>
		/// <param name="audioEngine"> The irrKlang audio engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned irrKlang audio engine. </returns>
		AudioEngineIrrKlang& operator=(const AudioEngineIrrKlang& audioEngine) = delete;

		/// <summary> irrKlang audio engine move assignment operator. </summary>
		/// <param name="audioEngine"> The irrKlang audio engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned irrKlang audio engine. </returns>
		AudioEngineIrrKlang& operator=(AudioEngineIrrKlang&& audioEngine) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		AUDIO_API void Update(math::Real deltaTime) override;
		AUDIO_API void LoadSoundEffect(const std::string& path) override;
		AUDIO_API void LoadSoundEffect3D(const std::string& path) override;
		AUDIO_API void LoadSong(const std::string& path) override;

		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */) override;
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch) override;
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity) override;
		AUDIO_API void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */) override;
		AUDIO_API void StopSoundEffects() override;
		AUDIO_API void StopSong() override;
		AUDIO_API void SetMasterVolume(math::Real volume) override;
		AUDIO_API void SetSoundEffectsVolume(math::Real volume) override;
		AUDIO_API void SetSongsVolume(math::Real volume) override;
	private:
		void Load(categories::Category type, const std::string& path);

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
		math::Real ChangeOctave(math::Real frequency, math::Real variation) const override;

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
		math::Real ChangeSemitone(math::Real frequency, math::Real variation) const override;

		void SetVolume(categories::Category type, math::Real volume);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		irrklang::ISoundEngine* m_engine;
		Filenames2SoundSources m_soundSources[categories::COUNT];

		irrklang::ISound* m_currentSong;
		std::string m_currentSongPath;
		std::string m_nextSongPath;
		
		fade_states::FadeState m_fade;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine_IRR_KLANG */

} /* end namespace audio */

#endif /* __AUDIO_AUDIO_ENGINE_IRR_KLANG_H__ */
