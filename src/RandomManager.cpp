#include "RandomManager.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

RandomManager::RandomManager()
{
    srand (time(NULL));
}

float RandomManager::GetValue(float min, float max, float precision )
{
    int newMin = static_cast<int>(min * precision);
    int newMax = static_cast<int>(max *precision);
    return GetValue(newMin, newMax) / precision;
}

int RandomManager::GetValue(int min, int max)
{
    if(min == max){return min;}
    if(min > max){return GetValue(max, min);}
    
    max = max+1;
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