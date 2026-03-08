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
void Plane::SetBulletsOrigin(int bullets)
{
	_bulletsOrigin = bullets;
}
void Plane::IncreaseBulletsOrigin(int bullets)
{
	_bulletsOrigin += bullets;
}
void Plane::IncreaseLives()
{
	_lives++;
}
void Plane::SetImmune(bool immune)
{
	_immune = immune;
}
bool Plane::IsImmune() const
{
	return _immune;
}

void Plane::Update(const float deltaTime)
{
	_currentAcumTime += deltaTime;

	if (_currentAcumTime > _fireRate)
	{
		for (int i = 0; i < _bulletsOrigin; ++i)
		{
			_callbackFire(i, this);
		}
		_currentAcumTime = 0;
	}
}
