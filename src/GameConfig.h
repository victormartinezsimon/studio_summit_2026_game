#pragma once
#include <array>
#include "Sprites.h"
#include "config/PoolConfig.h"
#include "config/MainMenuConfig.h"
#include "config/DefaultValuesConfig.h"
#include "config/ImprovementsConfig.h"
#include "config/MeteoritesConfiguration.h"
#include "config/StarsConfiguration.h"
#include "config/TimesConfig.h"
#include "config/EnemyConfiguration.h"
#include "config/ScoreConfiguration.h"
#include "config/CoordsConfig.h"
#include "config/ScreenConfig.h"
#include "config/BattleStateConfig.h"
#include "config/LevelConfiguration.h"

constexpr int MAX_SECS_PLAYING = 120;

#ifdef DEBUG
constexpr int NUM_FRAMES_TO_READ_INPUT = 1;
#else   
constexpr int NUM_FRAMES_TO_READ_INPUT = 4;
#endif
constexpr unsigned int MAX_PAINTED_OBJECTS = 500;

constexpr unsigned char TEAM_PLAYER = 1;
constexpr unsigned char TEAM_ENEMY = 2;
constexpr unsigned char TEAM_NEUTRAL = 0;

constexpr bool SHOW_TRAIL_BULLETS = true;
constexpr float TRAIL_LIVE_BULLETS = 0.3;

constexpr bool SHOW_TRAIL_METEORITE = false;
constexpr float TRAIL_LIVE_METEORITE = 0.3;

constexpr bool SHOW_TRAIL_STARS = true;
constexpr float TRAIL_LIVE_STARS = 0.3;

constexpr bool SHOW_FINAL_COUNTDOWN = true;
