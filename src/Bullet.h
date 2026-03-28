#pragma once
#include "WorldObject.h"

class Bullet : public WorldObject
{
public:
	Bullet();

public:
	void SetVelocity(float velX, float velY);
	void Update(const float deltaTime);
	void Paint(PainterManager* painter)override;


public:
	void SetHasPenetration(bool value);
	bool GetHasPenetration() const;

	void SetHasExplostion(bool value);
	bool GetHasExplostion() const;

private:
	float _velocityX = 0;
	float _velocityY = 0;
	bool _hasPenetration = false;
	bool _hasExplosion = false;
};