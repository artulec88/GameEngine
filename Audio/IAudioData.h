#ifndef __AUDIO_I_AUDIO_DATA_H__
#define __AUDIO_I_AUDIO_DATA_H__

#include "Audio.h"
#include "SampleInfo.h"

namespace audio
{
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
		AUDIO_API virtual ~IAudioData(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
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
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioData */

} /* end namespace Audio */

#endif /* __AUDIO_I_AUDIO_DATA_H__ */
