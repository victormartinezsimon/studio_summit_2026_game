#pragma once
#include "WorldObject.h"

class Bullet : public WorldObject
{
public:
	Bullet();

public:
	void SetVelocity(float velX, float velY);
	void Update(const float deltaTime);

public:
	void SetHasPenetration(bool value);
	bool GetHasPenetration() const;

	void SetHasExplostion(bool value);
	bool GetHasExplostion() const;

private:
	float _velocityX;
	float _velocityY;
	bool _hasPenetration;
	bool _hasExplosion;
};