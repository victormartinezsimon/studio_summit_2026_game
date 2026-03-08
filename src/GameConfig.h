#pragma once
#include <string_view>
#include <array>

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr unsigned int EXPECTED_FPS = 60;

constexpr unsigned int PLANES_POOL_SIZE = 11;
constexpr unsigned int BULLETS_POOL_SIZE = 50;

constexpr std::string_view BACKGROUND_IMAGE = "images/background.png";

constexpr int PLAYER_DEFAULT_BULLET_VEL_X = 0;
constexpr int PLAYER_DEFAULT_BULLET_VEL_Y = -50;
constexpr float PLAYER_DEFAULT_FIRE_RATE = 2;

constexpr int TOTAL_LEVELS_CONFIG = 5;
constexpr int MAX_ENEMIES_PER_ROW = 5;
constexpr std::array<int, TOTAL_LEVELS_CONFIG> LEVELS_CONFIGS = {2,3,5,8,PLANES_POOL_SIZE-1};

constexpr unsigned int ENEMY_WIDTH = 64;
constexpr unsigned int ENEMY_HEIGHT = 64;

