#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "RaylibPainter.h"

//#include "raylib-cpp.hpp" 

/*
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
*/
int main()
{
	/*
	Pool<Bullet, BULLETS_POOL_SIZE> bulletPool;
	Pool<Plane, PLANES_POOL_SIZE> enemiesPool;

	std::vector<Bullet*> currentBullets = InitTestBullets(bulletPool);

	TextPainter* painter = new TextPainter();
	painter->BuildScreen();

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

		painter->ClearScreen();
		for (auto&& b : currentBullets)
		{
			painter->PaintBullet(b);
		}
		painter->Paint();
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}
	return 0;
	*/

	/*
	int screenWidth = 800;
	int screenHeight = 450;

	raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");
	raylib::Texture logo("./images/amanecer.png");

	SetTargetFPS(60);

	while (!window.ShouldClose())
	{
		BeginDrawing();

		window.ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		// Object methods.
		logo.Draw(
			screenWidth / 2 - logo.GetWidth() / 2,
			screenHeight / 2 - logo.GetHeight() / 2);

		EndDrawing();
	}

	// UnloadTexture() and CloseWindow() are called automatically.

	//return 0;
	*/

	RaylibPainter p;

	Plane* player = new Plane();
	player->SetSize(PLANE_WIDTH, PLANE_HEIGHT);
	player->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9);

	Pool<Plane, PLANES_POOL_SIZE> enemiesPool;
	std::vector<Plane*> enemies;
	int totalEnemies = 5;
	for (int i = 0; i < totalEnemies; ++i)
	{
		auto enemy = enemiesPool.Get();
		int posX = SCREEN_WIDTH / (totalEnemies+1) * (i + 1);
		int posY = SCREEN_HEIGHT * 0.1;
		enemy->SetPosition(posX, posY);
		enemy->SetSize(ENEMY_WIDTH, ENEMY_HEIGHT);
		enemies.push_back( enemy );
	}

	while (!p.HasEnded())
	{
		p.BeginPaint();
		p.PaintBackground();
		p.PaintPlayer(player);

		for (auto&& enemy : enemies)
		{
			p.PaintEnemy(enemy);
		}

		p.EndPaint();
	}
}