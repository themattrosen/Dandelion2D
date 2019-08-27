#pragma once

#include <Core\Singleton.h>
#include <Core\Definitions.h>
#include <Core\Memory\Allocator.h>

#include <Audio\AudioVoice\AudioVoice.h>
#include <Audio\AudioEvents\AudioEvent.h>

#include <mutex>
#include <vector>
#include <unordered_map>

class AudioEngine : public Singleton<AudioEngine>
{
public:
	void Init();
	void Update();
	void Exit();

	f32 GetGlobalVolume() const;
	void SetGlobalVolume(f32 linear);
	void Mute();
	void UnMute();
	void ToggleMute();

	AudioVoice* PlaySound(string name, bool isLooped = false);
	void StopSound(AudioVoice* voice);
	void LoadWave(string filename, string eventName);

	

	static void Mix(f32* out, unsigned long frames, AudioEngine* engine);

	AudioEngine();

private:
	// port audio
	void* m_stream;
	std::mutex m_lock;

	// audio voice pool
	PoolAllocator<AudioVoice> m_voicePool;
	AudioVoice* m_playingList;

	// volume
	f32 m_globalVolume;
	bool m_isMuted;

	// assets
	std::unordered_map<string, WaveData*> m_loadedSounds;
	PoolAllocator<WaveData> m_soundPool;
};

namespace Dandelion
{
	DANDELION_API void LoadWave(string filename, string eventName);
	DANDELION_API AudioVoice* PlaySound(string name, bool isLooped = false);
	DANDELION_API void StopSound(AudioVoice* voice);
}
