#pragma once
#include <array>
#include "PainterManager.h"

class NumberManager 
{
public:
	enum class PIVOT {LEFT, RIGHT, CENTER};
public:
	NumberManager(PainterManager* painterManager);

public:
	void PaintNumber(int number, int x, int y, int minDigits, PIVOT pivot);
	
private:
	void PaintLeft(int number, int x, int y, int minDigits);
	void PaintRight(int number, int x, int y, int minDigits);
	void PaintCenter(int number, int x, int y, int minDigits);
	int GetSizeNumber(int number, int minDigits);

private:
	std::array<float, 10> _digitWidth;
	std::array<PainterManager::SPRITE_ID, 10> _spriteIDs;
	PainterManager *_painterManager;
};
