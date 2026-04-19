#include "Bullet.h"
#include <cmath>
#include "GameConfig.h"
#include "Sprites.h" //TODO: remove this inclusion
#include "PainterManager.h"

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

void Bullet::ConfigureSprite(PainterManager* painter)
{
	if(_isBig)
	{
		_spriteController.Configure(painter, PainterManager::SPRITE_ID::BULLET_BIG );
	}
	else
	{
		_spriteController.Configure(painter, PainterManager::SPRITE_ID::BULLET );
	}
}

void Bullet::SetBulletIsBig(bool value)
{
	_isBig = value;
}
bool Bullet::GetBulletIsBig() const
{
	return _isBig;
}
