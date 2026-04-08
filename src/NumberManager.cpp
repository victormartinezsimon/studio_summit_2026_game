#include "NumberManager.h"
#include "PainterManager.h"
#include "Sprites.h" //TODO: remove this inclusion

NumberManager::NumberManager(PainterManager* painterManager): _painterManager(painterManager)
{
	_numbersSprite.Configure(painterManager, PainterManager::SPRITE_ID::NUMBERS, 5, 2, -1);
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
	int currentValue = std::abs(number);
	int digitsUsed = 0;
	float w = _numbersSprite.GetWidth();
	int currentX = x + w/2;
	while(currentValue != 0)
	{
		int digit = currentValue % 10;
		currentValue = currentValue/ 10;
		++digitsUsed;

		currentX -= w;
		_numbersSprite.PaintFrame(_painterManager, currentX, y, digit);
	}

	while(digitsUsed < minDigits)
	{
		int digit = 0;
		++digitsUsed;
		currentX -= w;
		_numbersSprite.PaintFrame(_painterManager, currentX, y, digit);
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
	int currentValue = std::abs(number);
	int digitsUsed = 0;
	float w = _numbersSprite.GetWidth();
	while(currentValue != 0)
	{
		int digit = currentValue % 10;
		currentValue = currentValue/ 10;
		++digitsUsed;
		totalSize += w;
	}

	if(digitsUsed < minDigits)
	{
		int diff = minDigits - digitsUsed;
		totalSize += (diff * w);
	}
	return totalSize;
}

void NumberManager::GetSize(unsigned int& w, unsigned int& h)const
{
	_numbersSprite.GetSize(w, h);
}
unsigned int NumberManager::GetWidth()const
{
	return _numbersSprite.GetWidth();
}
unsigned int NumberManager::GetHeight()const
{
	return _numbersSprite.GetHeight();
}