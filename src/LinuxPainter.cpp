#include "LinuxPainter.h"
#include "Plane.h"
#include "Bullet.h"
#include <stdexcept>
#include <thread>

LinuxPainter::LinuxPainter()
{
	s_platform = SPInitPlatform();

	if (!s_platform)
	{
		throw std::runtime_error("failed creating platform");
	}

	stride = VPUGetStride(VIDEO_MODE, VIDEO_COLOR);
	frameBufferA.size = frameBufferB.size = stride * SCREEN_HEIGHT;
	SPAllocateBuffer(s_platform, &frameBufferA);
	SPAllocateBuffer(s_platform, &frameBufferB);

	VPUSetVideoMode(s_platform->vx, VIDEO_MODE, VIDEO_COLOR, EVS_Enable);
	init_palette(s_platform->vx);

	s_platform->sc->cycle = 0;
	s_platform->sc->framebufferA = &frameBufferA;
	s_platform->sc->framebufferB = &frameBufferB;

	VPUSetScanoutAddress(s_platform->vx, (uint32_t)frameBufferA.dmaAddress);
	VPUSetScanoutAddress2(s_platform->vx, (uint32_t)frameBufferB.dmaAddress);

	VPUSwapPages(s_platform->vx, s_platform->sc);
	VPUClear(s_platform->vx, 0x00000000);
	VPUSwapPages(s_platform->vx, s_platform->sc);
	VPUClear(s_platform->vx, 0x00000000);
}

LinuxPainter::~LinuxPainter()
{
}

void LinuxPainter::PaintBackground()
{
}

void LinuxPainter::BeginPaint()
{
	VPUSwapPages(s_platform->vx, s_platform->sc);

	dst = (uint8_t *)s_platform->sc->writepage;
	fill_background(dst, stride, 0);
}

void LinuxPainter::EndPaint()
{
	VPUSyncSwap(s_platform->vx, 0);
	VPUNoop(s_platform->vx);
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

float LinuxPainter::GetDeltaTime()
{
	return 0.16f;
}

bool LinuxPainter::HasEnded()
{
	return false;
}

void LinuxPainter::PaintPlayer(Plane *player)
{
	int x = player->GetX() - player->GetWidth()/2;
	int y = player->GetY() - player->GetHeight()/2;
	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite_player, player->GetWidth(), player->GetHeight(), x, y, 3);
}
void LinuxPainter::PaintEnemy(Plane *enemy)
{
	int x = enemy->GetX();
	int y = enemy->GetY();
	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite_cow, SPRITE_W, SPRITE_H, x, y, TRANSPARENT_KEY);
}

void LinuxPainter::PaintBullet(Bullet *bullet)
{
	int x = bullet->GetX() - bullet->GetWidth() / 2;
	int y = bullet->GetY() - bullet->GetHeight() / 2;

	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite_rabbit, SPRITE_W, SPRITE_H, x, y, TRANSPARENT_KEY);
}

void LinuxPainter::init_palette(struct EVideoContext *vctx)
{
	VPUSetDefaultPalette(vctx);
	/*
	// Animal colors
	VPUSetPal(vctx, 1, 255, 220, 50);  // Yellow (duck body)
	VPUSetPal(vctx, 2, 255, 140, 0);   // Orange (duck beak/feet)
	VPUSetPal(vctx, 3, 255, 255, 255); // White (cow/rabbit)
	VPUSetPal(vctx, 4, 20, 20, 20);	   // Black (cow spots, outlines)
	VPUSetPal(vctx, 5, 220, 100, 40);  // Orange-red (fox body)
	VPUSetPal(vctx, 6, 255, 200, 180); // Light peach (fox chest)
	VPUSetPal(vctx, 7, 255, 0, 0); // Gray (rabbit body)
	VPUSetPal(vctx, 8, 255, 150, 180); // Pink (rabbit ears/nose)
	for (int i = 0; i < 8; ++i)
	{
		uint8_t c = (uint8_t)(20 + i * 10);
		VPUSetPal(vctx, (uint8_t)(16 + i), c, c, (uint8_t)(40 + i * 8));
	}
	*/

	VPUSetPal(vctx, 0, 93,67,48);
	VPUSetPal(vctx, 1, 114,99,70);
	VPUSetPal(vctx, 2, 77,45,27);
	VPUSetPal(vctx, 3, 32,44,8);
	VPUSetPal(vctx, 4, 73,69,24);
	VPUSetPal(vctx, 5, 85,96,9);
	VPUSetPal(vctx, 6, 183,60,21);
	VPUSetPal(vctx, 7, 193,170,28);

	for (int i = 0; i < 8; ++i)
	{
		uint8_t c = (uint8_t)(20 + i * 10);
		VPUSetPal(vctx, (uint8_t)(16 + i), c, c, (uint8_t)(40 + i * 8));
	}

}

void LinuxPainter::fill_background(uint8_t *dst, uint32_t stride, uint32_t frame)
{
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		uint8_t *row = dst + (uint32_t)y * stride;
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			uint8_t v = (uint8_t)(16 + (((x + (int)(frame >> 1)) >> 4) & 7));
			row[x] = v;
		}
	}
}

void LinuxPainter::masked_blit_8(
	uint8_t *dst,
	uint32_t dst_stride,
	int dst_w,
	int dst_h,
	const uint8_t *src,
	int src_w,
	int src_h,
	int dst_x,
	int dst_y,
	uint8_t key)
#
{
	int src_x = 0;
	int src_y = 0;
	int w = src_w;
	int h = src_h;

	// Clip the source and destination rectangles to ensure we don't read/write out of screen bounds
	if (dst_x < 0)
	{
		src_x = -dst_x;
		dst_x = 0;
		w -= src_x;
	}
	if (dst_y < 0)
	{
		src_y = -dst_y;
		dst_y = 0;
		h -= src_y;
	}
	if (dst_x + w > dst_w)
		w = dst_w - dst_x;
	if (dst_y + h > dst_h)
		h = dst_h - dst_y;
	if (w <= 0 || h <= 0)
		return;

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
	uint8x16_t keyv = vdupq_n_u8(key); // Broadcast the key to 16 bytes
#endif

	for (int y = 0; y < h; ++y)
	{
		uint8_t *d = dst + (uint32_t)(dst_y + y) * dst_stride + dst_x;
		const uint8_t *s = src + (src_y + y) * src_w + src_x;
		int x = 0;

		// If NEON is available, we can process 16 pixels at a time.
		// The key is compared against the source pixels, and if it matches,
		// the destination pixel is kept; otherwise, the source pixel is copied to the destination.
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
		for (; x + 15 < w; x += 16)
		{
			uint8x16_t sv = vld1q_u8(s + x);		 // Load source pixels
			uint8x16_t dv = vld1q_u8(d + x);		 // Load destination pixels
			uint8x16_t mask = vceqq_u8(sv, keyv);	 // Compare source pixels with key, result is 0xFF where equal, 0x00 where not
			uint8x16_t out = vbslq_u8(mask, dv, sv); // If mask bit is 1, select from dv (keep dest), else select from sv (copy src)
			vst1q_u8(d + x, out);					 // Store result back to destination
		}
#endif
		// Process any remaining pixels that don't fit into a 16-byte block
		for (; x < w; ++x)
		{
			uint8_t px = s[x];
			if (px != key)
				d[x] = px;
		}
	}
}