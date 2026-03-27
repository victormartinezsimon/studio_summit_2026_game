#include "NumberManager.h"
#include "PainterManager.h"
#include "Sprites.h"

NumberManager::NumberManager(PainterManager* painterManager): _painterManager(painterManager)
{
	_digitWidth[0] = NUMBER_0_WIDTH;
	_digitWidth[1] = NUMBER_1_WIDTH;
	_digitWidth[2] = NUMBER_2_WIDTH;
	_digitWidth[3] = NUMBER_3_WIDTH;
	_digitWidth[4] = NUMBER_4_WIDTH;
	_digitWidth[5] = NUMBER_5_WIDTH;
	_digitWidth[6] = NUMBER_6_WIDTH;
	_digitWidth[7] = NUMBER_7_WIDTH;
	_digitWidth[8] = NUMBER_8_WIDTH;
	_digitWidth[9] = NUMBER_9_WIDTH;

	_spriteIDs[0] = PainterManager::SPRITE_ID::NUMBER_0;
	_spriteIDs[1] = PainterManager::SPRITE_ID::NUMBER_1;
	_spriteIDs[2] = PainterManager::SPRITE_ID::NUMBER_2;
	_spriteIDs[3] = PainterManager::SPRITE_ID::NUMBER_3;
	_spriteIDs[4] = PainterManager::SPRITE_ID::NUMBER_4;
	_spriteIDs[5] = PainterManager::SPRITE_ID::NUMBER_5;
	_spriteIDs[6] = PainterManager::SPRITE_ID::NUMBER_6;
	_spriteIDs[7] = PainterManager::SPRITE_ID::NUMBER_7;
	_spriteIDs[8] = PainterManager::SPRITE_ID::NUMBER_8;
	_spriteIDs[9] = PainterManager::SPRITE_ID::NUMBER_9;
}

void NumberManager::PaintNumber(int number, int x, int y, int minDigits, PIVOT pivot)
{
	switch (pivot)
	{
	case PIVOT::CENTER: PaintCenter(number, x, y, minDigits); break;
	case PIVOT::LEFT: PaintLeft(number, x, y, minDigits); break;
	case PIVOT::RIGHT: PaintRight(number, x, y, minDigits); break;
	}
}

void NumberManager::PaintLeft(int number, int x, int y, int minDigits)
{
	unsigned int totalSize = GetSizeNumber(number, minDigits);
	int newX = x + totalSize;
	PaintRight(number, newX, y, minDigits);
}

void NumberManager::PaintRight(int number, int x, int y, int minDigits)
{
	int currentValue = number;
	int digitsUsed = 0;
	float w_0, h_0;
	_painterManager->GetSpriteSize(_spriteIDs[0], w_0, h_0);
	int currentX = x + w_0/2;
	while(currentValue != 0)
	{
		int digit = currentValue % 10;
		currentValue = currentValue/ 10;
		++digitsUsed;

		float w, h;
		_painterManager->GetSpriteSize(_spriteIDs[digit], w, h);

		currentX -= w;
		_painterManager->AddToPaint(_spriteIDs[digit], currentX, y);
	}

	while(digitsUsed < minDigits)
	{
		int digit = 0;
		++digitsUsed;
		float w, h;
		_painterManager->GetSpriteSize(_spriteIDs[digit], w, h);
		currentX -= w;
		_painterManager->AddToPaint(_spriteIDs[digit], currentX, y);
	}
}
void NumberManager::PaintCenter(int number, int x, int y, int minDigits)
{
	unsigned int totalSize = GetSizeNumber(number, minDigits);
	int newX = x + totalSize/2;
	PaintRight(number, newX, y, minDigits);
}

int NumberManager::GetSizeNumber(int number, int minDigits)
{
unsigned int totalSize = 0;
	int currentValue = number;
	int digitsUsed = 0;
	while(currentValue != 0)
	{
		int digit = currentValue % 10;
		currentValue = currentValue/ 10;
		++digitsUsed;
		totalSize += _digitWidth[digit];
	}

	if(digitsUsed < minDigits)
	{
		int diff = minDigits - digitsUsed;
		totalSize += (diff * _digitWidth[0]);
	}
	return totalSize;
}