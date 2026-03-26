#pragma once
#include <array>
#include "PoolConfig.h"

constexpr int TOTAL_LEVELS_CONFIG = 5;
constexpr int MAX_ENEMIES_PER_ROW = 5;
constexpr std::array<int, TOTAL_LEVELS_CONFIG> LEVELS_CONFIGS = {1,3,5,8,PLANES_POOL_SIZE};
