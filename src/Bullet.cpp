#include "Bullet.h"
#include <cmath>
#include "GameConfig.h"

Bullet::Bullet():_velocityX(0), _velocityY(0)
{
	SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
}

void Bullet::SetVelocity(float velX, float velY)
{
	_velocityX = velX;
	_velocityY = velY;
}

void Bullet::Update(const float deltaTime)
{
	float newX = deltaTime * _velocityX;
	_X += newX;

	float newY = deltaTime * _velocityY;
	_Y += newY;
}