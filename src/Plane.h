#pragma once
#include <functional>
#include "WorldObject.h"
#include "PainterManager.h"

class TrailManager;

class Plane : public WorldObject
{
public:
	void Update(const float deltaTime) override;
	void Reset(float value);
	void Paint(PainterManager* painter) override;
	void ConfigureSprite(PainterManager* painter) override;
	
public:
	void SetCallbackFire(std::function<void(int, const Plane&)> fun);
	void SetTrailManager(TrailManager* trailManager);

public:
	void SetFireRate(float fireRate);
	void IncreaseFireRate(float increase);

public:
	void SetBulletsTotalSources(int bullets);

public:
	void SetBulletsPerShot(int bullets);
	int GetBulletsPerShot() const;

public:
	void SetHasShield(bool value);
	bool GetHasShield() const;

public:
	void SetBulletIsBig(bool value);
	bool GetBulletIsBig() const;

public:
	void SetTimeInmortal(float value);
	float GetTimeInmortal() const;

public:
	void SetRandomMovementID(int value);
	int GetRandomMovementID() const;

private:
	float CalculateDistanceSquared(float x1, float y1, float x2, float y2)const;
	void TryPaintTrail(PainterManager* painter);

private:
	std::function<void(int, const Plane&)> _callbackFire = nullptr;
	float _fireRate = 0;
	int _bulletsTotalSources = 1;
	float _currentAcumTime = 0;
	bool _hasShield = false;
	int _bulletsPerShot = 1;
	bool _bulletIsBig = false;
	float _timeInmortal = 0;
	int _randomMovementID = -1;
	TrailManager* _trailManager = nullptr;
	SpriteSheetController _spriteControllerShield;
	float _lastX = 0;
	float _lastY = 0;
	float _lastDeltaTime = 0;
};
