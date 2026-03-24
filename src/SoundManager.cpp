#include "SoundManager.h"
#include "space_chours.h"

SoundManager::SoundManager(struct SPPlatform* platform)
	: m_platform(platform)
	, m_ctx(nullptr)
	, m_apuBuffer{}
	, m_moduleLoaded(false)
	, m_stopRequested(false)
{
	m_apuBuffer.size = SM_BUFFER_BYTE_COUNT;
	SPAllocateBuffer(m_platform, &m_apuBuffer);
	memset(m_apuBuffer.cpuAddress, 0, SM_BUFFER_BYTE_COUNT);
}

SoundManager::~SoundManager()
{
	stop();

	if (m_moduleLoaded)
	{
		xmp_release_module(m_ctx);
		m_moduleLoaded = false;
	}
	if (m_ctx)
	{
		xmp_free_context(m_ctx);
		m_ctx = nullptr;
	}
	SPFreeBuffer(m_platform, &m_apuBuffer);
}

void SoundManager::start()
{
	m_stopRequested = false;
	m_thread = std::thread(&SoundManager::playThread, this);
}

void SoundManager::stop()
{
	m_stopRequested = true;
	if (m_thread.joinable())
		m_thread.join();
}

void SoundManager::playThread()
{
	m_ctx = xmp_create_context();

	if (xmp_load_module_from_memory(m_ctx, space_chours_data, space_chours_size) < 0)
	{
		printf("Error: cannot load embedded module\n");
		xmp_free_context(m_ctx);
		m_ctx = nullptr;
		return;
	}

	m_moduleLoaded = true;

	APUSetBufferSize(m_platform->ac, ABS_4096Bytes);
	APUSetSampleRate(m_platform->ac, ASR_22_050_Hz);

	printf("Checking device status\n");
	printf(" Word count:%d\n", APUGetWordCount(m_platform->ac));
	printf(" Cursor: %d\n", APUFrame(m_platform->ac));

	if (xmp_start_player(m_ctx, 22050, 0) == 0)
	{
		struct xmp_module_info mi;
		xmp_get_module_info(m_ctx, &mi);
		printf("%s (%s)\n", mi.mod->name, mi.mod->type);

		short* buf = (short*)m_apuBuffer.cpuAddress;
		while (!m_stopRequested)
		{
			if (xmp_play_buffer(m_ctx, buf, SM_BUFFER_BYTE_COUNT, 0) != 0)
				break;

			APUStartDMA(m_platform->ac, (uint32_t)m_apuBuffer.dmaAddress);
			APUWaitSync(m_platform->ac);

			std::this_thread::yield();
		}
		xmp_end_player(m_ctx);
	}
}

short* SoundManager::getAudioBuffer() const
{
	return (short*)m_apuBuffer.cpuAddress;
}
