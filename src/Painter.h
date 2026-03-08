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

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#define VIDEO_MODE EVM_320_240
#define VIDEO_COLOR ECM_8bit_Indexed

#define SPRITE_W 32
#define SPRITE_H 32
#define SPRITE_COUNT 4
#define TRANSPARENT_KEY 0

struct Sprite
{
	int x;
	int y;
	int vx;
	int vy;
};

static struct SPPlatform *s_platform = NULL;
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
	void PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

private:
	void init_palette(struct EVideoContext *vctx);
	void fill_background(uint8_t *dst, uint32_t stride, uint32_t frame);
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
		uint8_t key);

private:
	SPPlatform *s_platform;
	uint32_t stride;
	uint8_t *dst;
};

#endif