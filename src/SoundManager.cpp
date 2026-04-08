#include "SoundManager.h"
#include "background_sound.h"

SoundManager::SoundManager(struct SPPlatform* platform)
	: m_platform(platform)
	, m_ctx(nullptr)
	, m_apuBuffer{}
	, m_moduleLoaded(false)
	, m_stopRequested(false)
	, _ready(false)
{
	m_apuBuffer.size = SM_BUFFER_BYTE_COUNT;
	SPAllocateBuffer(m_platform, &m_apuBuffer);
	memset(m_apuBuffer.cpuAddress, 0, SM_BUFFER_BYTE_COUNT);
}

SoundManager::~SoundManager()
{
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
		//SPFreeBuffer(m_platform, &m_apuBuffer); // freed by platform shutdown
}

void SoundManager::start()
{
	m_stopRequested = false;
	m_thread = std::thread(&SoundManager::playThread, this);
}

void SoundManager::stop()
{
	if(m_stopRequested){return;}
	m_stopRequested = true;
	if (m_thread.joinable())
		m_thread.join();
}

void SoundManager::playThread()
{
	m_ctx = xmp_create_context();

	if (xmp_load_module_from_memory(m_ctx, space_alliance_mod, space_alliance_mod_len) < 0)
	{
		printf("Error: cannot load embedded module\n");
		xmp_free_context(m_ctx);
		m_ctx = nullptr;
		return;
	}

	m_moduleLoaded = true;

	APUSetBufferSize(m_platform->ac, ABS_4096Bytes);
	APUSetSampleRate(m_platform->ac, ASR_22_050_Hz);

	if (xmp_start_player(m_ctx, 22050, 0) == 0)
	{
		struct xmp_module_info mi;
		xmp_get_module_info(m_ctx, &mi);

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

void SoundManager::Update()
{
	if(!_ready){return;}

	if (xmp_play_buffer(m_ctx, buf, SM_BUFFER_BYTE_COUNT, 0) != 0)
	{
		_ready = false;
		return;
	}

	APUStartDMA(m_platform->ac, (uint32_t)m_apuBuffer.dmaAddress);
	APUWaitSync(m_platform->ac);
}

void SoundManager::Prepare()
{
	_ready = false;

	m_ctx = xmp_create_context();

	if (xmp_load_module_from_memory(m_ctx, space_alliance_mod, space_alliance_mod_len) < 0)
	{
		xmp_free_context(m_ctx);
		m_ctx = nullptr;
		return;
	}

	m_moduleLoaded = true;

	APUSetBufferSize(m_platform->ac, ABS_4096Bytes);
	APUSetSampleRate(m_platform->ac, ASR_22_050_Hz);

	if (xmp_start_player(m_ctx, 22050, 0) == 0)
	{
		struct xmp_module_info mi;
		xmp_get_module_info(m_ctx, &mi);
		buf = (short*)m_apuBuffer.cpuAddress;

		_ready = true;
	}
}