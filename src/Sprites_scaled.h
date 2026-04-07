#pragma once
#include "Sprites.h"

constexpr unsigned int BULLET_SMALL_WIDTH = BULLET_WIDTH/2;
constexpr unsigned int BULLET_SMALL_HEIGHT = BULLET_HEIGHT/2;
static uint8_t sprite_bullet_small[BULLET_SMALL_WIDTH * BULLET_SMALL_HEIGHT];

constexpr unsigned int BULLET_EXTRA_SMALL_WIDTH = BULLET_WIDTH/4;
constexpr unsigned int BULLET_EXTRA_SMALL_HEIGHT = BULLET_HEIGHT/4;
static uint8_t sprite_bullet_extra_small[BULLET_EXTRA_SMALL_WIDTH * BULLET_EXTRA_SMALL_HEIGHT];

constexpr unsigned int BULLET_BIG_WIDTH = BULLET_WIDTH*2;
constexpr unsigned int BULLET_BIG_HEIGHT = BULLET_HEIGHT*2;
static uint8_t sprite_bullet_big[BULLET_BIG_WIDTH * BULLET_BIG_HEIGHT];

constexpr unsigned int BULLET_EXTRA_BIG_WIDTH = BULLET_WIDTH*4;
constexpr unsigned int BULLET_EXTRA_BIG_HEIGHT = BULLET_HEIGHT*4;
static uint8_t sprite_bullet_extra_big[BULLET_EXTRA_BIG_WIDTH * BULLET_EXTRA_BIG_HEIGHT];