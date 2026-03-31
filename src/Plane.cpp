#include "Plane.h"
#include "GameConfig.h"
#include "SpriteSheetController.h"

void Plane::SetCallbackFire(std::function<void(int, const Plane&)> fun)
{
	_callbackFire = fun;
}
void Plane::SetFireRate(float fireRate)
{
	_fireRate = fireRate;
}
void Plane::IncreaseFireRate(float increase)
{
	_fireRate += increase;
}
void Plane::SetBulletsTotalSources(int bullets)
{
	_bulletsTotalSources = bullets;
}

void Plane::SetHasShield(bool value)
{
	_hasShield = value;
}
bool Plane::GetHasShield() const
{
	return _hasShield;
}

void Plane::SetBulletsPerShot(int bullets)
{
	_bulletsPerShot = bullets;
}
int Plane::GetBulletsPerShot() const
{
	return _bulletsPerShot;
}


void Plane::Update(const float deltaTime)
{
	_currentAcumTime += deltaTime;

	if(_timeInmortal > 0)
	{
		_timeInmortal-= deltaTime;
	}

	if (_currentAcumTime > _fireRate)
	{
		if(_bulletsTotalSources == DEFAULT_BULLETS_ORIGIN)
		{
			for (int i = 0; i < _bulletsTotalSources; ++i)
			{
				_callbackFire(i, *this);
			}
		}
		else
		{
			for (int i = 1; i < _bulletsTotalSources; ++i)
			{
				if(_callbackFire)
				{
					_callbackFire(i, *this);
				}
			}
		}
		_currentAcumTime = 0;
	}
}

void Plane::Reset(float value)
{
	_currentAcumTime = -value;
}

void Plane::SetTimeInmortal(float value)
{
	_timeInmortal = value;
}
float Plane::GetTimeInmortal() const
{
	return _timeInmortal;
}

void Plane::SetRandomMovementID(int value)
{
	_randomMovementID = value;
}
int Plane::GetRandomMovementID() const
{
	return _randomMovementID;
}

void Plane::Paint(PainterManager* painter)
{
	if(_playerTeam ==  TEAM_PLAYER)
	{
		Paint(painter, PainterManager::SPRITE_ID::PLAYER, PainterManager::SPRITE_ID::PLAYER_SHIELD);
	}

		if(_playerTeam ==  TEAM_ENEMY)
	{
		Paint(painter, PainterManager::SPRITE_ID::ENEMY, PainterManager::SPRITE_ID::ENEMY_SHIELD);
	}
}

void Plane::Paint(PainterManager* painter, PainterManager::SPRITE_ID spritePlane, PainterManager::SPRITE_ID spriteShield) const
{
	float currentTimeInmortal = GetTimeInmortal();
	if (currentTimeInmortal <= 0)
	{
		painter->AddToPaint(spritePlane, GetX(), GetY(), GetAlpha());
	}
	else
	{
		float percent = currentTimeInmortal / TIME_INMORTAL;

		float alpha = 0.5f;

		if ((percent >= 0.25 && percent <= 0.50) || (percent >= 0.75 && percent <= 1.0))
		{
			alpha = 0.1f;
		}

		painter->AddToPaint(spritePlane, GetX(), GetY(), alpha);
		//_spriteController.Paint(painter, GetX(),GetY());
	}

	if (GetHasShield())
	{
		_spriteControllerShield.Paint(painter, GetX(), GetY());
	}
}

void Plane::ConfigureSprite(PainterManager* painter)
{
	if(_playerTeam ==  TEAM_PLAYER)
	{
		_spriteController.Configure(painter, PainterManager::SPRITE_ID::PLAYER);
		_spriteControllerShield.Configure(painter, PainterManager::SPRITE_ID::PLAYER_SHIELD);
	}
	
	if(_playerTeam ==  TEAM_ENEMY)
	{
		_spriteController.Configure(painter, PainterManager::SPRITE_ID::ENEMY);
		_spriteControllerShield.Configure(painter, PainterManager::SPRITE_ID::ENEMY_SHIELD);
	}
}