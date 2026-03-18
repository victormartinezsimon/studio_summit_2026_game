#include "Painter.h"
#include <stdexcept>
#include <thread>

constexpr uint8_t TRANSPARENT_IDS[] = {  6,5,4,3 };  // your 3 IDs
constexpr uint8_t TRANSPARENT_COUNT = 4;

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

	halfMask = vreinterpretq_u8_u16(vdupq_n_u16(0x00FF));
	quarterMask = vreinterpretq_u8_u32(vdupq_n_u32(0x000000FF));
	allMask = vdupq_n_u8(0xFF);
}

Painter::~Painter()
{
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

void Painter::PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite, width, height, x, y, TRANSPARENT_IDS, TRANSPARENT_COUNT, allMask);
}

void Painter::PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, unsigned int x, unsigned int y, int maskType)
{
	uint8x16_t mask = allMask;
	if (maskType == 1)
	{
		mask = halfMask;
	}
	if (maskType == 2)
	{
		mask = quarterMask;
	}

	masked_blit_8(dst, stride, SCREEN_WIDTH, SCREEN_HEIGHT, sprite, width, height, x, y, TRANSPARENT_IDS, TRANSPARENT_COUNT, mask);
}

void Painter::init_palette(struct EVideoContext *vctx)
{
	VPUSetDefaultPalette(vctx);

	VPUSetPal(vctx,0,35,43,41);
	VPUSetPal(vctx,1,112,113,114);
	VPUSetPal(vctx,2,216,52,22);
	VPUSetPal(vctx,3,38,184,23);
	VPUSetPal(vctx,4,11,210,4);
	VPUSetPal(vctx,5,1,234,1);
	VPUSetPal(vctx,6,0,255,0);
	VPUSetPal(vctx,7,225,181,29);
	VPUSetPal(vctx,8,21,66,191);
	VPUSetPal(vctx,9,168,43,149);
	VPUSetPal(vctx,10,46,170,211);
	VPUSetPal(vctx,11,152,153,154);
	VPUSetPal(vctx,12,195,204,197);
	VPUSetPal(vctx,13,210,211,217);
	VPUSetPal(vctx,14,232,233,237);
	VPUSetPal(vctx,15,255,255,255);

	VPUSetPal(vctx, 16, 8, 15, 42); // background
}

void Painter::masked_blit_8(
	uint8_t *dst,
	uint32_t dst_stride,
	int dst_w,
	int dst_h,
	const uint8_t *src,
	int src_w,
	int src_h,
	int dst_x,
	int dst_y,
	const uint8_t *transparent_ids,  // <-- array instead of single value
    uint8_t transparent_count,
	uint8x16_t extraAlphaMask)
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
	uint8x16_t keyv0 = vdupq_n_u8(transparent_ids[0]);
    uint8x16_t keyv1 = vdupq_n_u8(transparent_ids[1]);
    uint8x16_t keyv2 = vdupq_n_u8(transparent_ids[2]);
    uint8x16_t keyv3 = vdupq_n_u8(transparent_ids[3]);
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

			// Apply extra alpha mask: where mask byte is 0x00, replace src pixel with transparent value
    		 // Apply extra alpha mask first
            sv = vbslq_u8(extraAlphaMask, sv, keyv0);

            // Build transparency mask: 0xFF where pixel matches ANY transparent id
            uint8x16_t tmask = vceqq_u8(sv, keyv0);
            tmask = vorrq_u8(tmask, vceqq_u8(sv, keyv1));
            tmask = vorrq_u8(tmask, vceqq_u8(sv, keyv2));
            tmask = vorrq_u8(tmask, vceqq_u8(sv, keyv3));

			uint8x16_t out = vbslq_u8(tmask, dv, sv); // transparent -> keep dst, else copy src
            vst1q_u8(d + x, out);
		}
#endif
		// Process any remaining pixels that don't fit into a 16-byte block
		for (; x < w; ++x)
		{
			uint8_t px = s[x];
			bool is_transparent = false;
            for (int i = 0; i < transparent_count; ++i)
                is_transparent |= (px == transparent_ids[i]);
            if (!is_transparent)
                d[x] = px;
		}
	}
}
