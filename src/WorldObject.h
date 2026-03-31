#pragma once
#include <utility>
#include "SpriteSheetController.h"

class PainterManager;


class WorldObject
{
public:
	virtual void Paint(PainterManager* painter)
	{
		_spriteController.Paint(painter, GetX(), GetY(), _alpha);
	}
	
	virtual void ConfigureSprite(PainterManager* painter){};
	virtual void Update(const float deltaTime)
	{
		_spriteController.Update(deltaTime);
	};

public:
	float GetX() const
	{
		return _X;
	}

	float GetY() const
	{
		return _Y;
	}

	void SetPosition(float x, float y)
	{
		_X = x;
		_Y = y;
	}

	void SetPositionX(float x)
	{
		_X = x;
	}

	void SetPositionY(float y)
	{
		_Y = y;
	}

public:
	unsigned int GetWidth() const
	{
		return _width;
	}
	unsigned int GetHeight() const
	{
		return _height;
	}
	void SetSize(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
	}

public:
	unsigned char  GetPlayerTeam() const
	{
		return _playerTeam;
	}
	void SetPlayerTeam(unsigned char  playerTeam)
	{
		_playerTeam = playerTeam;
	}

public:
	void SetID(int id)
	{
		_id = id;
	}
	int GetID() const
	{
		return _id;
	}

public:
	void SetAlpha(float alpha)
	{
		_alpha = alpha;
	}
	int GetAlpha() const
	{
		return _alpha;
	}

public:
	SpriteSheetController* GetSpriteController()
	{
		return &_spriteController;
	}

protected:
	float _X = 0;
	float _Y = 0;
	float _alpha = 1;
	unsigned int _width = 0;
	unsigned int _height = 0;

	unsigned char _playerTeam = 0;
	int _id = -1;
	SpriteSheetController _spriteController;
};