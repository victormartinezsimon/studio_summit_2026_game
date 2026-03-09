#pragma once
#include <functional>
#include "WorldObject.h"

class Plane : public WorldObject
{
public:
	void Update(const float deltaTime);

public:
	void SetCallbackFire(std::function<void(int, Plane*)> fun);

public:
	void SetFireRate(float fireRate);
	void IncreaseFireRate(float increase);

public:
	void SetBulletsTotalSources(int bullets);

public:
	void SetHasShield(bool value);
	bool GetHasShield() const;

private:
	std::function<void(int, Plane*)> _callbackFire;
	float _fireRate = 0;
	float _bulletsTotalSources = 1;
	float _currentAcumTime = 0;
	bool _hasShield = false;
};
