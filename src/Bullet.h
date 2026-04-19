#pragma once
#include "WorldObject.h"

class Bullet : public WorldObject
{
public:
	Bullet();

public:
	void Update(const float deltaTime)override;
	void ConfigureSprite(PainterManager* painter) override;

public:
void SetVelocity(float velX, float velY);

public:
	void SetHasPenetration(bool value);
	bool GetHasPenetration() const;

	void SetHasExplostion(bool value);
	bool GetHasExplostion() const;

	void SetBulletIsBig(bool value);
	bool GetBulletIsBig() const;

private:
	float _velocityX = 0;
	float _velocityY = 0;
	bool _hasPenetration = false;
	bool _hasExplosion = false;
	bool _isBig = false;
};