#ifndef __AUDIO_IAUDIO_ENGINE_H__
#define __AUDIO_IAUDIO_ENGINE_H__

#include "Audio.h"
#include "Math\Vector.h"
#include <string>

namespace Audio
{
	namespace Categories
	{
		enum Category
		{
			SOUND_EFFECT = 0,
			SOUND_EFFECT_3D,
			SONG,
			COUNT // The number of categories
		}; /* end enum Category */
	} /* end namespace Categories */

	namespace FadeStates
	{
		enum FadeState
		{
			FADE_NONE = 0,
			FADE_IN,
			FADE_OUT
		}; /* end enum FadeState */
	} /* end namespace FadeStates */

	/// <summary>
	/// The interface for the audio engine.
	/// </summary>
	class IAudioEngine
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Audio engine constructor.
		/// </summary>
		AUDIO_API IAudioEngine();
		/// <summary>
		/// Audio engine destructor.
		/// </summary>
		AUDIO_API virtual ~IAudioEngine(void);
	private:
		IAudioEngine(const IAudioEngine& audioEngine);
		void operator=(const IAudioEngine& audioEngine);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Updates the state of the audio engine. For irrKlang audio engine running in multithreaded mode this function may be an empty operation.
		/// </summary>
		AUDIO_API virtual void Update(Math::Real deltaTime) = 0;
		AUDIO_API virtual void LoadSoundEffect(const std::string& path) = 0;
		AUDIO_API virtual void LoadSoundEffect3D(const std::string& path) = 0;
		AUDIO_API virtual void LoadSong(const std::string& path) = 0;

		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. If the sound under the specified key has not been loaded yet, the function simply returns.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */) = 0;
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. If the sound under the specified key has not been loaded yet, the function simply returns.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch) = 0;
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */,
			Math::Real volume, Math::Real pitch, const Math::Vector3D& position, const Math::Vector3D& velocity) = 0;
		AUDIO_API virtual void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */) = 0;
		AUDIO_API virtual void StopSoundEffects() = 0;
		AUDIO_API virtual void StopSong() = 0;
		AUDIO_API virtual void SetMasterVolume(Math::Real volume) = 0;
		AUDIO_API virtual void SetSoundEffectsVolume(Math::Real volume) = 0;
		AUDIO_API virtual void SetSongsVolume(Math::Real volume) = 0;
	protected:
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
		virtual Math::Real ChangeOctave(Math::Real frequency, Math::Real variation) const = 0;

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
		virtual Math::Real ChangeSemitone(Math::Real frequency, Math::Real variation) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioEngine */

} /* end namespace Audio */

#endif /* __AUDIO_IAUDIO_ENGINE_H__ */
