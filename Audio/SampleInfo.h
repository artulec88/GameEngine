#ifndef __AUDIO_SAMPLE_INFO_H__
#define __AUDIO_SAMPLE_INFO_H__

#include "Audio.h"

namespace audio
{
	/// <summary>
	/// Holds various properties of the audio, e.g. volume, pitch, loop, panning. They tell how we play the audio data.
	/// </summary>
	class SampleInfo
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit SampleInfo(double volume, double pitch);
		AUDIO_API ~SampleInfo();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		double m_volume;
		double m_pitch;
		// TODO: loop information, panning etc.
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SampleInfo */

} /* end namespace audio */

#endif /* __AUDIO_SAMPLE_INFO_H__ */
