#include "AudioEngine.h"
#include <portaudio\portaudio.h>

static int OnWrite(const void* vin, void* vout, unsigned long frames,
	const PaStreamCallbackTimeInfo* tinfo,
	PaStreamCallbackFlags flags, void* user);

void AudioEngine::Init()
{
	Pa_Initialize();

	PaStreamParameters outputParams;
	outputParams.device = Pa_GetDefaultOutputDevice();
	ASSERT(outputParams.device != paNoDevice);
	outputParams.channelCount = 2;
	outputParams.sampleFormat = paFloat32;
	outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultHighOutputLatency;
	outputParams.hostApiSpecificStreamInfo = 0;

	Pa_OpenStream(&m_stream, 0, &outputParams, 44100.0, 0, paClipOff, OnWrite, this);
	ASSERT(m_stream != nullptr);
	Pa_StartStream(m_stream);
}

#include <iostream>

void AudioEngine::Update()
{
}

void AudioEngine::Exit()
{
	Pa_StopStream(m_stream);
	Pa_CloseStream(m_stream);
	Pa_Terminate();
}

f32 AudioEngine::GetGlobalVolume() const
{
	return m_globalVolume;
}

void AudioEngine::SetGlobalVolume(f32 linear)
{
	m_globalVolume = linear;
}

void AudioEngine::Mute()
{
	m_isMuted = true;
}

void AudioEngine::UnMute()
{
	m_isMuted = false;
}

void AudioEngine::ToggleMute()
{
	m_isMuted = !m_isMuted;
}

AudioVoice* AudioEngine::PlaySound(string name, bool isLooped)
{
	m_lock.lock();

	AudioVoice* voice = nullptr;
	auto sound = m_loadedSounds.find(name);
	if (sound == m_loadedSounds.end())
	{
		std::cout << "Couldn't post event for sound: " << name << std::endl;
	}
	else
	{
		WaveData* asset = sound->second;
		voice = m_voicePool.Construct(asset);
		voice->isLooping = isLooped;
		voice->next = m_playingList;
		if (m_playingList) m_playingList->prev = voice;
		m_playingList = voice;
		voice->isPlaying = true;
	}

	m_lock.unlock();

	return voice;
}

void AudioEngine::StopSound(AudioVoice * voice)
{
	m_lock.lock();

	if(voice)
		voice->isPlaying = false;

	m_lock.unlock();
}

void AudioEngine::LoadWave(string filename, string eventName)
{
	if (m_loadedSounds.find(eventName) == m_loadedSounds.end())
	{
		WaveData* wave = m_soundPool.Construct(filename);
		m_loadedSounds.insert_or_assign(eventName, wave);
	}
	else
	{
		std::cout << "A wave file has already been loaded with event name " << eventName << std::endl;
	}
}

int OnWrite(const void* vin, void* vout, unsigned long frames,
	const PaStreamCallbackTimeInfo* tinfo,
	PaStreamCallbackFlags flags, void* user)
{
	f32* out = reinterpret_cast<f32*>(vout);
	AudioEngine* engine = reinterpret_cast<AudioEngine *>(user);
	
	AudioEngine::Mix(out, frames, engine);
	return 0;
}

void AudioEngine::Mix(f32 * out, unsigned long frames, AudioEngine * engine)
{
	std::memset(out, 0, size_t(sizeof(float) * frames * 2));
	f32 volume = engine->m_globalVolume * (engine->m_isMuted) ? 0.f : 1.f;

	AudioVoice** playingList = &engine->m_playingList;
	AudioVoice* playing = *playingList;

	engine->m_lock.lock();

	while (playing)
	{
		playing->ProcessFrames(out, frames);

		if (!playing->isPlaying)
		{
			AudioVoice* toDelete = playing;
			// remove from playing list
			if (playing->prev)
			{
				playing->prev->next = playing->next;
				if (playing->next) playing->next->prev = playing->prev;
			}
			else
			{
				engine->m_playingList = playing->next;
				if (engine->m_playingList) engine->m_playingList->prev = nullptr;
			}

			playing = playing->next;

			engine->m_voicePool.Free(toDelete);
		}
		else
		{
			playing = playing->next;
		}
	}

	engine->m_lock.unlock();
}

AudioEngine::AudioEngine() :
	m_stream(nullptr),
	m_lock(),
	m_voicePool(MAX_CONCURRENT_AUDIO_VOICES, false),
	m_playingList(nullptr),
	m_globalVolume(1.f),
	m_isMuted(false),
	m_loadedSounds(),
	m_soundPool(MAX_AUDIO_ASSETS, false)
{}

static AudioEngine& AE = AudioEngine::Instance();

void Dandelion::LoadWave(string filename, string eventName)
{
	AE.LoadWave(filename, eventName);
}

AudioVoice * Dandelion::PlaySound(string name, bool isLooped)
{
	return AE.PlaySound(name, isLooped);
}

void Dandelion::StopSound(AudioVoice * voice)
{
	AE.StopSound(voice);
}
