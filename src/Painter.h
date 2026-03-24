#pragma once
#ifndef _WIN32
#include <array>
#include "GameConfig.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "platform.h"
#include "vpu.h"
#include <array>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#define VIDEO_MODE EVM_320_240
#define VIDEO_COLOR ECM_8bit_Indexed

struct Sprite
{
	int x;
	int y;
	int vx;
	int vy;
};

static struct SPPlatform *s_platform;
static struct SPSizeAlloc frameBufferA;
static struct SPSizeAlloc frameBufferB;

class Painter
{
public:
	Painter();
	~Painter();

public:
	void BeginPaint();
	void EndPaint();

public:
	void PaintBackground();
	void PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, int x, int y);
	void PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, int x, int y, int maskType);

public:
 struct SPPlatform* GetPlatform(){return s_platform;}

private:
	void init_palette(struct EVideoContext *vctx);
	void masked_blit_8(
		uint8_t *dst,
		uint32_t dst_stride,
		int dst_w,
		int dst_h,
		const uint8_t *src,
		int src_w,
		int src_h,
		int dst_x,
		int dst_y,
		const uint8_t transparent_id,
		uint8x16_t evenRowMask, uint8x16_t oddRowMask);

private:
	SPPlatform *s_platform;
	uint32_t stride;
	uint8_t *dst;
	uint8x16_t allMask;
	uint8x16_t halfMask;
	uint8x16_t halfMaskAlt;
	uint8x16_t quarterMask;
	uint8x16_t quarterMaskAlt;
};

#endif