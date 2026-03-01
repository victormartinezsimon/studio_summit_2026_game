#pragma once
#include <utility>
#include "WorldObject.h"

class Bullet:public WorldObject
{
public:
	Bullet();

public:
	bool GetTeam() const;
	void SetTeam(bool playerTeam);

public:
	void SetVelocity(float vel);
	void Update(const float deltaTime);


private:
	float _velocityY;
	bool _playerTeam;
};