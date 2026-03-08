#pragma once
#include <utility>
#include "WorldObject.h"

class Bullet:public WorldObject
{
public:
	Bullet();

public:
	void SetVelocity(float velX, float velY);
	void Update(const float deltaTime);


private:
	float _velocityX;
	float _velocityY;
};