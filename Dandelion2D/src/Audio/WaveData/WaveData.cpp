#include "WaveData.h"
#include <fstream>

WaveData::WaveData(string filename) :
	m_samplingRate(0.f),
	m_sizeInBytes(0),
	m_numSamples(0),
	m_data(nullptr),
	m_bitsPerSample(0),
	m_numChannels(0)
{
	ReadWave(filename, *this);
}

WaveData::~WaveData()
{
	if (m_data)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

void WaveData::ReadWave(string filename, WaveData & adata)
{
	std::fstream in(filename, std::ios_base::binary | std::ios_base::in);
	ASSERT(in.good() && "File doesn't exist!");

	char header[44];
	in.read(header, 44);
	u32 size = *reinterpret_cast<u32*>(header + 40);
	ASSERT(size && "Empty WAV file!");
	u32 rate = *reinterpret_cast<u32*>(header + 24);
	adata.m_bitsPerSample = *reinterpret_cast<u16*>(header + 34);
	ASSERT(adata.m_bitsPerSample == 16 && "Dandelion2D only supports 16 bit audio files");

	char* data = new char[size];
	adata.m_samplingRate = static_cast<float>(rate);
	adata.m_sizeInBytes = size;
	adata.m_numSamples = size / (adata.m_bitsPerSample / 8);
	adata.m_numChannels = *reinterpret_cast<u16*>(header + 22);
	ASSERT(adata.m_numChannels == 1 || adata.m_numChannels == 2 
		&& "Dandelion2D only supports mono or stereo audio files");
	
	in.read(data, size);

	adata.m_data = new float[adata.m_numSamples];
	const f32 denom = static_cast<f32>((1 << (adata.m_bitsPerSample - 1)));

	for (u32 i = 0; i < adata.m_numSamples; ++i)
	{
		s16 val = *reinterpret_cast<s16 *>(data + i * sizeof(s16));

		f32 nextVal = static_cast<f32>(val) / denom;
		adata.m_data[i] = nextVal;
	}

	delete[] data;
}
