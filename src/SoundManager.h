#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <atomic>

#include "core.h"
#include "platform.h"
#include "apu.h"
#include "xmp.h"

// Number of 16bit stereo samples (valid values are 1024, 512, 256, 128 or 64)
#define SM_BUFFER_SAMPLE_COUNT 1024
#define SM_BUFFER_CHANNEL_COUNT 2
#define SM_BUFFER_SAMPLE_SIZE sizeof(short)
#define SM_BUFFER_BYTE_COUNT (SM_BUFFER_SAMPLE_COUNT * SM_BUFFER_SAMPLE_SIZE * SM_BUFFER_CHANNEL_COUNT)

class SoundManager
{
public:
	SoundManager(struct SPPlatform* platform);
	~SoundManager();

	// Start playback on a background thread
	void start();

	// Stop playback and join the thread
	void stop();

	// Access the current audio buffer for visualization
	short* getAudioBuffer() const;

	// Get the number of samples per buffer
	uint32_t getBufferSampleCount() const { return SM_BUFFER_SAMPLE_COUNT; }

private:
	void playThread();

	struct SPPlatform* m_platform;
	xmp_context m_ctx;
	SPSizeAlloc m_apuBuffer;
	bool m_moduleLoaded;
	std::thread m_thread;
	std::atomic<bool> m_stopRequested;
};
