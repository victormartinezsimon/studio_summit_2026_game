#pragma once
#include <string_view>

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr unsigned int EXPECTED_FPS = 60;

constexpr unsigned int PLANES_POOL_SIZE = 20;
constexpr unsigned int BULLETS_POOL_SIZE = 200;

constexpr std::string_view BACKGROUND_IMAGE = "images/background.png";

constexpr int PLAYER_DEFAULT_BULLET_VEL_X = 0;
constexpr int PLAYER_DEFAULT_BULLET_VEL_Y = -10;
constexpr float PLAYER_DEFAULT_FIRE_RATE = 20;

constexpr unsigned int ENEMY_WIDTH = 50;
constexpr unsigned int ENEMY_HEIGHT = 50;