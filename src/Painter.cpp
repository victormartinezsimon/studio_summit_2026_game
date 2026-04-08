#include "Painter.h"
#include <stdexcept>
#include <thread>

constexpr int TRANSPARENT_KEY = 4;

Painter::Painter()
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

	// Initialize opacity masks using ordered dithering threshold pattern
	// threshold[row][col] = (row + col * 3) % 10
	// For opacity level L (1-10): pixel is visible if threshold < L
	for (int level = 0; level < 10; ++level)
	{
		int L = level + 1; // opacity level 1-10
		for (int row = 0; row < 10; ++row)
		{
			uint8_t pattern[16];
			for (int col = 0; col < 16; ++col)
			{
				int threshold = (row + col * 3) % 10;
				pattern[col] = (threshold < L) ? 0xFF : 0x00;
			}
			opacityMasks[level][row] = vld1q_u8(pattern);
		}
	}
}

Painter::~Painter()
{
	SPFreeBuffer(s_platform, &frameBufferA);
	SPFreeBuffer(s_platform, &frameBufferB);
	SPShutdownPlatform(s_platform);	
}

void Painter::BeginPaint()
{
	// Ensure VPU fifo is empty
	while (VPUGetFIFONotEmpty(s_platform->vx))
	{
	}
	// Swap frames
	VPUSwapPages(s_platform->vx, s_platform->sc);
}

void Painter::EndPaint()
{
	// Add a buffer swap commmand
	VPUSyncSwap(s_platform->vx, 0);
	// Insert a no-operation command (barrier) that we can wait on
	VPUNoop(s_platform->vx);
}

void Painter::PaintBackground()
{
	dst = (uint8_t *)s_platform->sc->writepage;
	VPUClear(s_platform->vx, 0x10101010);
}

void Painter::PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, int x, int y, 
	int opacityPercent, int startX, int startY, int fullImageWidth, int fullImageHeight)
{
	int opacityLevel = opacityPercent / 10;
	if (opacityLevel < 1) opacityLevel = 1;
	if (opacityLevel > 10) opacityLevel = 10;

	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite, width, height, x, y, startX, startY, 
		fullImageWidth, fullImageHeight, TRANSPARENT_KEY, 
		opacityLevel);
}

void Painter::init_palette(struct EVideoContext *vctx)
{
	VPUSetDefaultPalette(vctx);

	VPUSetPal(vctx,0,30,13,26);
	VPUSetPal(vctx,1,246,9,20);
	VPUSetPal(vctx,2,236,22,30);
	VPUSetPal(vctx,3,218,65,17);
	//VPUSetPal(vctx,4,0,255,0);
	VPUSetPal(vctx,5,254,198,25);
	VPUSetPal(vctx,6,27,45,212);
	VPUSetPal(vctx,7,13,62,230);
	VPUSetPal(vctx,8,15,58,232);
	VPUSetPal(vctx,9,14,80,203);
	VPUSetPal(vctx,10,35,173,246);
	VPUSetPal(vctx,11,218,217,179);
	VPUSetPal(vctx,12,217,216,217);
	VPUSetPal(vctx,13,238,238,241);
	VPUSetPal(vctx,14,255,255,255);

	VPUSetPal(vctx, 16, 8, 15, 42); // background

	VPUSetPal(vctx, 17, 107, 127, 174); // fat star
	VPUSetPal(vctx, 18, 191, 215, 255); // mid star
	VPUSetPal(vctx, 19, 255, 255, 255); // near star

}

void Painter::masked_blit_8(
	uint8_t *dst,
	uint32_t dst_stride,
	int screen_width,
	int screen_height,
	const uint8_t *sprite,
	int sprite_width,
	int sprite_height,
	int screen_x,
	int screen_y,
	int startX,
	int startY,
	int full_image_widht,
	int full_image_height,
	const uint8_t transparent_key, 
	int opacityLevel)
{
	int src_x = startX;
	int src_y = startY;
	int w = sprite_width;
	int h = sprite_height;

	// Clip the source and destination rectangles to ensure we don't read/write out of screen bounds
	if (screen_x < 0)
	{
		src_x = -screen_x;
		screen_x = 0;
		w -= src_x;
	}
	if (screen_y < 0)
	{
		src_y = -screen_y;
		screen_y = 0;
		h -= src_y;
	}
	if (screen_x + w > screen_width)
		w = screen_width - screen_x;
	if (screen_y + h > screen_height)
		h = screen_height - screen_y;
	if (w <= 0 || h <= 0)
		return;

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
	uint8x16_t keyv0 = vdupq_n_u8(transparent_key);
#endif

	for (int y = 0; y < h; ++y)
	{
		uint8_t *d = dst + (uint32_t)(screen_y + y) * dst_stride + screen_x;
		//const uint8_t *s_1 = sprite + (src_y + y) * sprite_width + src_x;
		const uint8_t *s = sprite + (src_y + y) * full_image_widht + src_x;

		int row_in_pattern = (screen_y + y) % 10;
		uint8x16_t extraAlphaMask = opacityMasks[opacityLevel - 1][row_in_pattern];

		int x = 0;

		// If NEON is available, we can process 16 pixels at a time.
		// The key is compared against the source pixels, and if it matches,
		// the destination pixel is kept; otherwise, the source pixel is copied to the destination.
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
		for (; x + 15 < w; x += 16)
		{
			uint8x16_t sv = vld1q_u8(s + x);		 // Load source pixels
			uint8x16_t dv = vld1q_u8(d + x);		 // Load destination pixels

			// Apply extra alpha mask: where mask byte is 0x00, replace sprite pixel with transparent value
    		 // Apply extra alpha mask first
            sv = vbslq_u8(extraAlphaMask, sv, keyv0);

            // Build transparency mask: 0xFF where pixel matches ANY transparent id
            uint8x16_t tmask = vceqq_u8(sv, keyv0);

			uint8x16_t out = vbslq_u8(tmask, dv, sv); // transparent -> keep dst, else copy sprite
            vst1q_u8(d + x, out);
		}
#endif
		// Process any remaining pixels that don't fit into a 16-byte block
		for (; x < w; ++x)
		{
			//TODO: review this lines
			int col = x & 15;
			int threshold_val = (row_in_pattern + col * 3) % 10;
			if (threshold_val >= opacityLevel) continue;
			//TODO: end review
			uint8_t px = s[x];
			if (px != TRANSPARENT_KEY)
				d[x] = px;
		}
	}
}
