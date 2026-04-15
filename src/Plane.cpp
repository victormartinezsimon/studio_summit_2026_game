#include "Plane.h"
#include "GameConfig.h"
#include "SpriteSheetController.h"
#include "TrailManager.h"


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
	_lastDeltaTime = deltaTime;
	_spriteController.Update(deltaTime);

	if(_timeInmortal > 0)
	{
		_timeInmortal-= deltaTime;

		float currentTimeInmortal = GetTimeInmortal();
		
		float percent = currentTimeInmortal / TIME_INMORTAL;

		_alpha = 0.5f;

		if ((percent >= 0.25 && percent <= 0.50) || (percent >= 0.75 && percent <= 1.0))
		{
			_alpha = 0.1f;
		}
	}
	else
	{
		_alpha= 1.0f;
	}

	if (_currentAcumTime > _fireRate)
	{
		if(_bulletsTotalSources == DEFAULT_BULLETS_ORIGIN)
		{
			if(_callbackFire)
			{
				_callbackFire(0, *this);
			}
		}
		else
		{
			//skip the center bullet
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
	if (GetHasShield())
	{
		_spriteControllerShield.Paint(painter, GetX(), GetY());
	}
	_spriteController.Paint(painter, GetX(),GetY(), _alpha);

	TryPaintTrail(painter);
}

void Plane::TryPaintTrail(PainterManager* painter)
{
	if(_trailManager == nullptr){return;}
	
	if(_lastX == 0 && _lastY == 0){return;}//exceptional case that happens during initialization

	float distance = CalculateDistanceSquared(_X, _Y, _lastX, _lastY);
	float velocity = distance / _lastDeltaTime;

	if(velocity > TRAIL_PLANE_MIN_VELOCITY)
	{
		int id = _trailManager->AddTrail(painter, _lastX, _lastY, 
			_spriteController.GetWidth(), _spriteController.GetHeight(), TRAIL_PLANE_LIVE, _spriteController.GetSprite());		
		
		if(id != -1)
		{
			_trailManager->GetSpriteSheetNormal(id)->Configure(painter, &_spriteController);
		}
	}

	_lastX = _X;
	_lastY = _Y;
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

void Plane::SetTrailManager(TrailManager* trailManager)
{
	_trailManager = trailManager;
}

float Plane::CalculateDistanceSquared(float x1, float y1, float x2, float y2)const
{
	float diffX = x1 - x2;
	float diffY = y1 - y2;
	return diffX * diffX + diffY* diffY;
}
