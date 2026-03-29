#pragma once
#include <map>
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
						NUMBER_0,NUMBER_1,NUMBER_2,NUMBER_3,NUMBER_4,NUMBER_5,NUMBER_6,
						NUMBER_7,NUMBER_8,NUMBER_9,
						METEORITE, EXPLOSION, ENEMY_SHIELD, PLAYER_SHIELD,
						NEAR_STAR, MID_STAR, FAR_STAR,
						FINAL_SCORE, RETURN_MENU, EXIT_GAME,
						LETTERS
					};
	enum class MASK_ID {FULL, HALF, QUARTER};

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
	void GetSpriteSize(SPRITE_ID id, float& width, float& height)const;
	float GetWidth(SPRITE_ID id)const;
	float GetHeight(SPRITE_ID id)const;

public:
	void ClearListPaint();

	void AddToPaint(SPRITE_ID id, float x, float y);
	void AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask);
	void AddToPaint(SPRITE_ID id, float x, float y, unsigned int width, unsigned int height);
	void AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask, unsigned int width, unsigned int height);
	void AddToPaint(SPRITE_ID id, float x, float y, MASK_ID mask, unsigned int width, unsigned int height, int spriteCoordX, int spriteCoordY);

private:
	int GetMaskID(MASK_ID maskID);

private:
	Painter* _painter;
	std::map<SPRITE_ID, const uint8_t*> _sprites;
	std::map<SPRITE_ID, std::pair<int, int>> _sizes;
	std::array<data, MAX_PAINTED_OBJECTS> _toPaint;
	int _currentIndexToPaint;
};