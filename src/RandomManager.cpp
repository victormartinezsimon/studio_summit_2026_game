#include "RandomManager.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

RandomManager::RandomManager()
{
    srand (time(NULL));
}

float RandomManager::GetValue(float min, float max, float precision = 1000.0f)
{
    int newMin = static_cast<int>(min * precision);
    int newMax = static_cast<int>(max *precision);
    return GetValue(newMin, newMax) / precision;
}

int RandomManager::GetValue(int min, int max)
{
    return min + rand() % (max - min);
}

int RandomManager::GetNextIntValue()
{
    return rand();
}

float RandomManager::GetNextFloatValue()
{
    return static_cast<float>(rand());
}