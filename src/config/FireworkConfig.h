#pragma once
#include "ScreenConfig.h"


constexpr float FIREWORK_TIME_SPAWN = 3;
constexpr float FIREWORK_ASCEND_VELOCITY = -50;
constexpr float FIREWORK_DURATION_TRAIL_MOVING = 0.5;
constexpr float FIREWORK_DURATION_TRAIL_EXPLOSION = 0.3;
constexpr float FIREWORK_GRAVITY = 15;
constexpr float FIREWORK_INITIAL_VELOCITY = 30;
constexpr int FIREWORK_TOTAL_AMOUNT = 5;
constexpr float FIREWORK_MIN_HEIGHT = SCREEN_HEIGHT * 0.2;
constexpr float FIREWORK_MAX_HEIGHT= SCREEN_HEIGHT * 0.2;
constexpr float FIREWORK_MIN_APPEAR_X = SCREEN_WIDTH * 0.2;
constexpr float FIREWORK_MAX_APPEAR_X = SCREEN_WIDTH * 0.7;
constexpr float FIREWORK_TIME_EXPLOSION_LIVE = 5;
constexpr float FIREWORK_MIN_PERCENT_FOR_EXPLOSION_TRAIL = 0.1;