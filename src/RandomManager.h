#pragma once
#include <array>
#include <utility>
#include "GameConfig.h"
#include <random>

class RandomManager
{
    public:
        RandomManager();
        float GetValue(float min, float max, float precision);
        int GetValue(int min, int max);
        int GetNextIntValue();
        float GetNextFloatValue();
};