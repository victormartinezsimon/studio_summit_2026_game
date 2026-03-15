#include "Bullet.h"
#include <cmath>
#include "GameConfig.h"
#include "Sprites.h"

Bullet::Bullet() : _velocityX(0), _velocityY(0)
{
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

void Bullet::SetHasPenetration(bool value)
{
	_hasPenetration = value;
}

bool Bullet::GetHasPenetration() const
{
	return _hasPenetration;
}

void Bullet::SetHasExplostion(bool value)
{
	_hasExplosion = value;
}

bool Bullet::GetHasExplostion() const
{
	return _hasExplosion;
}