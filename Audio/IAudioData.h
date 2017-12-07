#ifndef __AUDIO_I_AUDIO_DATA_H__
#define __AUDIO_I_AUDIO_DATA_H__

#include "Audio.h"

namespace audio
{
	class SampleInfo;

	/// <summary>
	/// Low-level representation of audio data.
	/// </summary>
	class IAudioData
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API IAudioData();
		AUDIO_API virtual ~IAudioData();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Low-level generation of samples for an audio.
		/// </summary>
		/// <param name="stream">The stream of audio data.</param>
		/// <param name="streamLength">The length of the stream of audio data.</param>
		/// <param name="pos">The position to start playing at.</param>
		/// <param name="sampleInfo">Sample information. It represents various alteration to how the audio should be played.</param>
		AUDIO_API virtual size_t GenerateSamples(float* stream, size_t streamLength, size_t pos, const SampleInfo& sampleInfo) = 0;
		AUDIO_API virtual size_t GetAudioLength() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioData */

} /* end namespace audio */

#endif /* __AUDIO_I_AUDIO_DATA_H__ */
