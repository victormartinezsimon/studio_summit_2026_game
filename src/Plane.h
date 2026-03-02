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

public:
	void SetBulletsToFire(int bullets);

public:
	void IncreaseLives();

public:
	void SetImmune(bool immune);
	bool IsImmune() const;

private:
	std::function<void(int, Plane*)> _callbackFire;
	float _fireRate = 0;
	float _bulletsToFire = 1;
	unsigned int _lives = 0;
	bool _immune = false;
	float _currentAcumTime = 0;
};
