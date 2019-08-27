#pragma once

#include <Core\Definitions.h>
#include <Core\Math\MathCommon.h>
#include <Audio\WaveData\WaveData.h>

class WaveData;

struct AudioSample
{
	f32 left;
	f32 right;
};

struct AudioVoice
{
	AudioVoice(WaveData* asset) :
		next(nullptr),
		prev(nullptr),
		wave(asset),
		index(0.f),
		pitchModifier(1.f),
		volume(1.f),
		pan(0.f),
		isPlaying(false),
		isLooping(false)
	{}

	AudioVoice* next;
	AudioVoice* prev;
	WaveData* wave;
	f32 index;
	f32 pitchModifier;
	f32 volume;
	f32 pan; // 0.0 for center, 1 for right, -1 for left
	bool isPlaying;
	bool isLooping;

	void SetPitch(s32 centsFromBase)
	{
		pitchModifier = std::pow(2.f, f32(centsFromBase) / 1200.f);
	}

	__forceinline void ProcessFrames(f32* outPtr, u32 numFrames)
	{
		f32* out = outPtr;
		bool stopPlaying = false;
		switch (wave->GetChannelCount())
		{
			case 1:
			{
				f32* dataPtr = wave->GetRawData();
				u32 startingIndex = static_cast<u32>(index);
				f32 fraction = index - startingIndex;
				f32* data = dataPtr + startingIndex;
				u32 end = wave->GetNumSamples();
				for (u32 i = 0; i < numFrames && u32(index) < end; ++i)
				{
					// TODO: deal with panning?
					f32 next = LERP_FLOAT(*data, *(data + 1), fraction);

					// left
					*out++ += next;

					// right
					*out++ += next;

					index += pitchModifier;
					startingIndex = static_cast<u32>(index);
					fraction = index - startingIndex;
					data = dataPtr + startingIndex;
				}

				if (u32(index) == end)
				{
					stopPlaying = true;
				}

				break;
			}
			case 2:
			{
				f32* dataPtr = wave->GetRawData();
				u32 startingFrame = static_cast<u32>(index);
				u32 startingIndex = SAMPLE_INDEX(startingFrame, 0, 2);
				f32 fraction = index - startingFrame;
				f32* data = dataPtr + startingIndex;
				u32 end = wave->GetNumSamples() / 2;
				for (u32 i = 0; i < numFrames && u32(index) < end; ++i)
				{
					f32 left = LERP_FLOAT(*data++, *(++data), fraction);
					f32 right = LERP_FLOAT(*((--data)++), *(++data), fraction);

					*out++ += left;
					*out++ += right;

					index += pitchModifier;
					startingFrame = static_cast<u32>(index);
					fraction = index - startingFrame;
					startingIndex = SAMPLE_INDEX(startingFrame, 0, 2);
					data = dataPtr + startingIndex;
				}

				if (u32(index) == end)
				{
					stopPlaying = true;
				}
				break;
			}
		}

		if (stopPlaying)
		{
			if (!isLooping)
			{
				isPlaying = false;
			}
			else
			{
				index = 0.f;
			}
		}
	}

	__forceinline constexpr AudioSample NextSample()
	{
		AudioSample next = { 0.f, 0.f };
		if (isPlaying)
		{
			switch (wave->GetChannelCount())
			{
				case 1:
				{
					f32 sample = wave->NextSample(index, 0) * volume;
					// TODO: deal with panning here
					
					next.left = sample;
					next.right = sample;
					break;
				}
				case 2:
				{
					f32 l = wave->NextSample(index, 0) * volume;
					f32 r = wave->NextSample(index, 1) * volume;

					// TODO: deal with panning here too

					next.left = l;
					next.right = r;
					break;
				}
			}

			index += pitchModifier;
			if (index + 1 >= f32(wave->GetNumSamples()))
			{
				if (!isLooping)
				{
					isPlaying = false;
				}
				else
				{
					index = 0.f;
				}
			}
		}

		return next;
	}

};
