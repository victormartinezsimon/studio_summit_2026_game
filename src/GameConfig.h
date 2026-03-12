#pragma once
#include <string_view>
#include <array>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

constexpr unsigned int PLANES_POOL_SIZE = 11;
constexpr unsigned int BULLETS_POOL_SIZE = 50;
constexpr unsigned int MAX_EASING_VALUES = PLANES_POOL_SIZE + 1;

constexpr float MAIN_MENU_MIN_VALUE = 0.3;
constexpr float MAIN_MENU_MAX_VALUE = 0.7;

constexpr float INTIAL_ANIMATION_DURATION = 1;

constexpr float DEFAULT_BULLET_VEL_X = 0;
constexpr float DEFAULT_BULLET_VEL_Y = 50;
constexpr float DEFAULT_FIRE_RATE = 2;
constexpr int DEFAULT_BULLETS_ORIGIN = 1;
constexpr bool DEFAULT_BULLET_HAS_PENETRATION = false;
constexpr bool DEFAULT_BULLET_HAS_EXPLOSION = false;
constexpr bool DEFAULT_HAS_SHIELD = false;
constexpr int DEFAULT_BULLETS_PER_SHOT = 1;
constexpr float EXPLOSION_SIZE= 30;

constexpr int TOTAL_LEVELS_CONFIG = 5;
constexpr int MAX_ENEMIES_PER_ROW = 5;
constexpr std::array<int, TOTAL_LEVELS_CONFIG> LEVELS_CONFIGS = {1,3,5,8,PLANES_POOL_SIZE-1};

constexpr std::string_view IMPROVEMENT_3_SHOTS = "3SHOTS";
constexpr std::string_view IMPROVEMENT_INCREASE_ORIGIN = "MORE_ORIGIN";
constexpr std::string_view IMPROVEMENT_INCREASE_FIRE_RATE = "MORE_FIRE_RATE";
constexpr std::string_view IMPROVEMENT_GIVE_PENETRATION = "PENETRATION";
constexpr std::string_view IMPROVEMENT_GIVE_EXPLOSION = "EXPLOSION";
constexpr std::string_view IMPROVEMENT_GIVE_SHIELD = "SHIELD";
constexpr std::string_view IMPROVEMENT_FAST_SHOTS = "FAST_SHOTS";
constexpr std::string_view IMPROVEMENT_SLOW_SHOTS = "SLOW_SHOTS";

constexpr float INCREASE_FIRE_RATE = 0.8f;
constexpr float FAST_SHOT_MULTIPLICATION = 1.5;
constexpr float SLOW_SHOT_MULTIPLICATION = 0.8;
constexpr float VALUE_MORE_SHOTS_X = 20;

constexpr int TOTAL_IMPROVEMENTS_TO_SELECT = 4;
