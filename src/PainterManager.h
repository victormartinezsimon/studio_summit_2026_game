#pragma once
#include <vector>
#include <map>
#include <stdint.h>//uing8_t

class Painter;

class PainterManager
{
public:
	enum class SPRITE_ID{PLAYER, ENEMY, BULLET};

private:
	struct data
	{
		SPRITE_ID id;
		unsigned int width;
		unsigned int height;
		unsigned int x;
		unsigned int y;
	};
public:
	PainterManager();
	~PainterManager();
public:
	void Paint() const;

public:
	void ClearListPaint();
	void AddToPaint(SPRITE_ID id, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

private:
	Painter* _painter;
	std::vector<data> _toPaint;
	std::map<SPRITE_ID, const uint8_t*> _sprites;
};