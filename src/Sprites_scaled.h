#pragma once
#include "Sprites.h"

constexpr unsigned int BULLET_SMALL_WIDTH = BULLET_WIDTH/2;
constexpr unsigned int BULLET_SMALL_HEIGHT = BULLET_HEIGHT/2;
static uint8_t sprite_bullet_small[BULLET_SMALL_WIDTH * BULLET_SMALL_HEIGHT];

constexpr unsigned int BULLET_BIG_WIDTH = BULLET_WIDTH*2;
constexpr unsigned int BULLET_BIG_HEIGHT = BULLET_HEIGHT*2;
static uint8_t sprite_bullet_big[BULLET_BIG_WIDTH * BULLET_BIG_HEIGHT];

constexpr unsigned int NUMBERS_BIG_WIDTH = NUMBERS_WIDTH*2;
constexpr unsigned int NUMBERS_BIG_HEIGHT = NUMBERS_HEIGHT*2;
static uint8_t sprite_numbers_big[NUMBERS_BIG_WIDTH * NUMBERS_BIG_HEIGHT];