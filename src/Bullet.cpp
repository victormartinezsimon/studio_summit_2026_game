#include "Bullet.h"
#include <cmath>
#include "GameConfig.h"

Bullet::Bullet():_velocityY(0)
{
	SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
}

void Bullet::SetVelocity(float vel)
{
	_velocityY = vel;
}

void Bullet::Update(const float deltaTime)
{
	float newY = deltaTime * _velocityY;
	_Y += newY;
}