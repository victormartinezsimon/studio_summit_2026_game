#pragma once
#include <string_view>
#include <array>

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
constexpr std::array<int, TOTAL_IMPROVEMENTS_TO_SELECT> LEVELS_WITH_IMPROVEMENT_SELECTION = {0,2,3,5};
constexpr int TOTAL_DEFINED_IMPROVEMENTS = 8;
