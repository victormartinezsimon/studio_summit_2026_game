#include "Plane.h"

void Plane::SetCallbackFire(std::function<void(int, Plane*)> fun)
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

void Plane::Update(const float deltaTime)
{
	_currentAcumTime += deltaTime;

	if (_currentAcumTime > _fireRate)
	{
		for (int i = 0; i < _bulletsTotalSources; ++i)
		{
			_callbackFire(i, this);
		}
		_currentAcumTime = 0;
	}
}
