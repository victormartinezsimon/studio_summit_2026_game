#pragma once
#include <array>

enum class ImprovementID
{
	SHOTS_3,
	INCREASE_ORIGIN,
	INCREASE_FIRE_RATE,
	GIVE_PENETRATION,
	GIVE_EXPLOSION,
	GIVE_SHIELD,
	FAST_SHOTS,
	BULLET_BIG
};

constexpr float INCREASE_FIRE_RATE = 0.8f;
constexpr float FAST_SHOT_MULTIPLICATION = 1.5;
constexpr float SLOW_SHOT_MULTIPLICATION = 0.8;
constexpr float VALUE_MORE_SHOTS_X = 20;
constexpr int SHOTS_IN_3_SHOTS = 3;
constexpr int NEW_EXTRA_SOURCES = 3;

constexpr int TOTAL_IMPROVEMENTS_TO_SELECT = 4;
constexpr std::array<int, TOTAL_IMPROVEMENTS_TO_SELECT> LEVELS_WITH_IMPROVEMENT_SELECTION = {0,2,3,5};
constexpr int TOTAL_DEFINED_IMPROVEMENTS = 8;
