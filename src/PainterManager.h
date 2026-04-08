#pragma once
#include <unordered_map>
#include <stdint.h>//uing8_t
#include <array>
#include "GameConfig.h"

class Painter;

class PainterManager
{
public:
	enum class SPRITE_ID{PLAYER, ENEMY, BULLET, 
						TITLE, START_GAME,
						PLAYER_SELECTOR, ENEMY_SELECTOR,
						SHOT_3_TIMES,INCREASE_ORIGIN,INCRASE_FIRE_RATE,GIVE_PENETRATION,
						GIVE_EXPLOSION,GIVE_SHIELD,FAST_SHOTS,SLOW_SHOTS,
						NUMBERS,
						METEORITE, EXPLOSION, ENEMY_SHIELD, PLAYER_SHIELD,
						NEAR_STAR, MID_STAR, FAR_STAR,
						FINAL_SCORE, RETURN_MENU, EXIT_GAME,
						LETTERS
					};
private:
	struct data
	{
		SPRITE_ID id;
		unsigned int width;
		unsigned int height;
		float x;
		float y;
		unsigned int mask;
		int spriteCoordX;
		int spriteCoordY;
	};
public:
	PainterManager();
	~PainterManager();
public:
	void Paint() const;

	Painter* GetPainter(){return _painter;}
	void GetSpriteSize(SPRITE_ID id, unsigned int& width, unsigned int& height)const;
	unsigned int GetWidth(SPRITE_ID id)const;
	unsigned int GetHeight(SPRITE_ID id)const;

public:
	void ClearListPaint();

	void AddToPaint(SPRITE_ID id, float x, float y);
	void AddToPaint(SPRITE_ID id, float x, float y, float alpha);
	void AddToPaint(SPRITE_ID id, float x, float y, float alpha, 
		unsigned int width, unsigned int height, int spriteCoordX, int spriteCoordY);

private:
	int GetMaskID(float alpha);

private:
	Painter* _painter;
	std::unordered_map<SPRITE_ID, const uint8_t*> _sprites;
	std::unordered_map<SPRITE_ID, std::pair<unsigned int, unsigned int>> _sizes;
	std::array<data, MAX_PAINTED_OBJECTS> _toPaint;
	int _currentIndexToPaint = 0;
};