#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "TextPainter.h"

#include <windows.h>
void setConsoleSize(short width, short height) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Tamaño del buffer
	COORD bufferSize = { width, height };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	// Tamaño de la ventana
	SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

std::vector<Bullet*> InitTestBullets(Pool<Bullet, BULLETS_POOL_SIZE>& bulletPool)
{
	std::vector<Bullet*> currentBullets;

	for (int i = 0; i < 2; ++i)
	{
		auto bullet = bulletPool.Get();
		bullet->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		bullet->SetPlayerTeam(i % 2);
		if (i % 2 == 0)
		{
			bullet->SetVelocity(-6);
		}
		else
		{
			bullet->SetVelocity(10);
		}

		currentBullets.push_back(bullet);
	}
	return currentBullets;
}

int main()
{
	setConsoleSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Pool<Bullet, BULLETS_POOL_SIZE> bulletPool;
	Pool<Plane, PLANES_POOL_SIZE> enemiesPool;

	std::vector<Bullet*> currentBullets = InitTestBullets(bulletPool);

	TextPainter painter;

	int milliseconds = 1000/30;

	while (!currentBullets.empty())
	{
		std::vector<Bullet*> toRemove;
		for (int i = 0; i < currentBullets.size(); ++i)
		{
			currentBullets[i]->Update(milliseconds/ 1000.0);
			if (currentBullets[i]->GetY() <= 0)
			{
				toRemove.push_back(currentBullets[i]);
			}

			if (currentBullets[i]->GetY() + BULLETS_HEIGHT >= SCREEN_HEIGHT)
			{
				toRemove.push_back(currentBullets[i]);
			}
		}
		for (auto&& bullet : toRemove)
		{
			auto it = std::find(currentBullets.begin(), currentBullets.end(), bullet);
			if (it != currentBullets.end())
			{
				currentBullets.erase(it);
				bulletPool.Release(bullet);
				std::cout << "returning bullet\n";
			}
			else
			{
				throw std::runtime_error("Trying to remove a non existing bullet");
			}
		}

		painter.ClearScreen();
		for (auto&& b : currentBullets)
		{
			painter.PaintBullet(b);
		}
		painter.Paint();
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	return 0;
}