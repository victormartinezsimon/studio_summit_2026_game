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
	void PaintItem(const uint8_t *sprite, unsigned int width, unsigned int height, int x, int y, int opacityPercent,
		 int startX, int startY, int fullImageWidth, int fullImageHeight);

public:
 struct SPPlatform* GetPlatform(){return s_platform;}

private:
	void init_palette(struct EVideoContext *vctx);
	void masked_blit_8(
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
		const uint8_t transparent_id,
		int opacityLevel);

private:
	SPPlatform *s_platform;
	struct SPSizeAlloc frameBufferA;
	struct SPSizeAlloc frameBufferB;
	uint32_t stride;
	uint8_t *dst;
	uint8x16_t opacityMasks[10][10]; // [level_index 0-9][row_pattern 0-9]
};

#endif