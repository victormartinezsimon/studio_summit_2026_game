#pragma once
#include <string_view>
#include <array>

constexpr int MAX_SECS_PLAYING = 120;

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

constexpr unsigned int PLANES_POOL_SIZE = 11;
constexpr unsigned int BULLETS_POOL_SIZE = 100;
constexpr unsigned int MAX_EASING_VALUES = PLANES_POOL_SIZE + 1;
constexpr unsigned int MAX_ALPHA_VALUES = 20;
constexpr unsigned int MAX_PAINTED_OBJECTS = 100;

constexpr unsigned char TEAM_PLAYER = 1;
constexpr unsigned char TEAM_ENEMY = 2;
constexpr unsigned char TEAM_NEUTRAL = 0;

constexpr float MAIN_MENU_MAX_VALUE = 0.7;
constexpr float MAIN_MENU_TIME_TO_ENTER = 2;

constexpr float POSITION_Y_PLAYER = 204;

constexpr float INTIAL_ANIMATION_DURATION = 1;

constexpr float DEFAULT_BULLET_VEL_X = 0;
constexpr float DEFAULT_BULLET_VEL_Y = 70;
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
constexpr int SHOTS_IN_3_SHOTS = 3;
constexpr int NEW_EXTRA_SOURCES = 3;

constexpr int TOTAL_IMPROVEMENTS_TO_SELECT = 4;
constexpr float TIME_TO_SELECT_IMPROVEMENT = 2;
constexpr int TOTAL_DEFINED_IMPROVEMENTS = 8;

constexpr int SCORE_PER_KILL = 50;
constexpr int SCORE_PER_PLAYER_HIT = 10;
constexpr int SCORE_PER_FINISH_LEVEL = 100;

constexpr int TOTAL_EXPLOSIONS = 10;
constexpr int EXPLOSION_DURATION = 2;

constexpr float ALPHA_TIME_DESTROY_PLANE = 0.8;
constexpr float ALPHA_TIME_ENTER_GAME = 1;

constexpr float TIME_INMORTAL = 2;

constexpr int TOTAL_METEORITES = 6;
constexpr float MIN_VELOCITY_METEORITE = 0.3;
constexpr float MAX_VELOCITY_METEORITE = 0.7;
constexpr float MIN_HEIGHT_METEORITE = 0.3;
constexpr float MAX_HEIGHT_METEORITE = 0.7;
constexpr float TIME_SPAWN_METEORITE = 3;

constexpr int TOTAL_STARS = 50;
constexpr float TIME_SPAWN_STAR = 0.7;
constexpr float VELOCITY_STAR_FAR = 0.2;
constexpr float VELOCITY_STAR_MID = 0.5;
constexpr float VELOCITY_STAR_NEAR = 1.2;
constexpr float MIN_HEIGHT_STAR = -0.5;
constexpr float MAX_HEIGHT_STAR = 1.5;
constexpr float MIN_VELOCITY_STAR = 80;
constexpr float MAX_VELOCITY_STAR = 180;
