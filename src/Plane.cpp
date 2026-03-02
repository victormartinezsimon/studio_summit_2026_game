#include "Plane.h"

void Plane::SetCallbackFire(std::function<void(int, Plane*)> fun)
{
	_callbackFire = fun;
}
void Plane::SetFireRate(float fireRate)
{
	_fireRate = fireRate;
}
void Plane::SetBulletsToFire(int bullets)
{
	_bulletsToFire = bullets;
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
		for (int i = 0; i < _bulletsToFire; ++i)
		{
			_callbackFire(i, this);
		}
		_currentAcumTime = 0;
	}
}
