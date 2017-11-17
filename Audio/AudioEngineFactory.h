#ifndef __AUDIO_AUDIO_ENGINE_FACTORY_H__
#define __AUDIO_AUDIO_ENGINE_FACTORY_H__

#include "Audio.h"

#include <memory>
#include <string>

namespace audio
{
	class IAudioEngine;

	namespace audio_engine_types
	{
		enum AudioEngineType
		{
			FMOD = 0,
			IRR_KLANG,
			COUNT // the number of audio engine types supported
		}; /* end enum AudioEngineType */
	} /* end namespace audio_engine_types */

	class AudioEngineFactory
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Audio engine factory constructor. </summary>
		/// <param name="audioDirectory"> The system location where all audio files are located. </param>
		AUDIO_API explicit AudioEngineFactory(const std::string& audioDirectory);

		/// <summary> Audio engine destructor. </summary>
		AUDIO_API ~AudioEngineFactory();
		
		/// <summary> Audio engine factory copy constructor. </summary>
		/// <param name="audioEngineFactory"> The audio engine factory to copy construct from. </param>
		AudioEngineFactory(const AudioEngineFactory& audioEngineFactory) = delete;

		/// <summary> Audio engine factory move constructor. </summary>
		/// <param name="audioEngineFactory"> The audio engine factory to move construct from. </param>
		AudioEngineFactory(AudioEngineFactory&& audioEngineFactory) = delete;

		/// <summary> Audio engine factory copy assignment operator. </summary>
		/// <param name="audioEngineFactory"> The audio engine factory to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned audio engine factory. </returns>
		AudioEngineFactory& operator=(const AudioEngineFactory& audioEngineFactory) = delete;

		/// <summary> Audio engine factory move assignment operator. </summary>
		/// <param name="audioEngineFactory"> The audio engine factory to move assign from. </param>
		/// <returns> The reference to the newly move-assigned audio engine factory. </returns>
		AudioEngineFactory& operator=(AudioEngineFactory&& audioEngineFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		AUDIO_API std::unique_ptr<IAudioEngine> CreateAudioEngine(audio_engine_types::AudioEngineType audioEngineType = audio_engine_types::IRR_KLANG) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const std::string& m_audioDirectory;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioEngine */

} /* end namespace audio */

#endif /* __AUDIO_AUDIO_ENGINE_FACTORY_H__ */
