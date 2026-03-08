#pragma once
#include <string_view>

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr unsigned int EXPECTED_FPS = 60;

constexpr unsigned int PLANES_POOL_SIZE = 20;
constexpr unsigned int BULLETS_POOL_SIZE = 200;

constexpr std::string_view BACKGROUND_IMAGE = "images/background.png";

constexpr unsigned int PLAYER_BULLET_VEL_Y = -10;

constexpr std::string_view ENEMY_IMAGE = "images/enemy.png";
constexpr unsigned int ENEMY_WIDTH = 50;
constexpr unsigned int ENEMY_HEIGHT = 50;

constexpr std::string_view BULLET_IMAGE = "images/bullet.png";
constexpr unsigned int BULLETS_WIDTH = 10;
constexpr unsigned int BULLETS_HEIGHT = 10;