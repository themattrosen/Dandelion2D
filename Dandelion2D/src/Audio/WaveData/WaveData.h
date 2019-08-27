#pragma once

#include <Core\Definitions.h>
#include <Core\Math\MathCommon.h>

#define SHORT_TO_FLOAT(s) (static_cast<float>(s) / static_cast<float>((1 << (16 - 1))))
#define FLOAT_TO_SHORT(s) (static_cast<short>(s * static_cast<float>((1 << (16 - 1)) - 1)))
#define SAMPLE_INDEX(frame, channel, numChannels) ((frame) * (numChannels) + (channel))
#define SAMPLE_CHANNEL(data, frame, channel, numChannels)	\
	data[SAMPLE_INDEX(frame, channel, numChannels)]

class WaveData
{
public:
	WaveData(string filename);
	~WaveData();

	static void ReadWave(string filename, WaveData& adata);

	__forceinline f32 NextSample(f32 fractionalIndex, u32 channel)
	{
		// no error checking
		f32 bottomIndex = std::floor(fractionalIndex);
		f32 fraction = fractionalIndex - bottomIndex;
		f32 prev = SAMPLE_CHANNEL(m_data, u32(bottomIndex), channel, m_numChannels);
		f32 next = SAMPLE_CHANNEL(m_data, u32(bottomIndex) + 1, channel, m_numChannels);
		return LERP_FLOAT(prev, next, fraction);
	}

	__forceinline f32 GetSamplingRate() const { return m_samplingRate; }
	__forceinline f32* GetRawData() { return m_data; }
	__forceinline u16 GetChannelCount() const { return m_numChannels; }
	__forceinline u32 GetNumSamples() const { return m_numSamples; }

private:

	f32 m_samplingRate;
	u32 m_sizeInBytes;
	u32 m_numSamples;
	f32* m_data;
	u16 m_bitsPerSample;
	u16 m_numChannels;
};