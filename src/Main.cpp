#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "RaylibPainter.h"

int main()
{
	RaylibPainter p;

	Plane* player = new Plane();
	player->SetSize(PLANE_WIDTH, PLANE_HEIGHT);
	player->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.9);
	player->SetBulletsToFire(1);
	player->SetFireRate(2);

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

	Pool<Bullet, BULLETS_POOL_SIZE> bulletsPool;
	std::vector<Bullet*> bullets;

	auto spawnPlayerBullet = [&bulletsPool, &bullets](int index, Plane* p)
		{
			auto bullet = bulletsPool.Get();
			bullet->SetPosition(p->GetX(), p->GetY());
			bullet->SetVelocity(-100);
			bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
			bullets.push_back(bullet);
		};
	//auto spawnEnemyBullet = [&bullets]() {};

	player->SetCallbackFire(spawnPlayerBullet);


	while (!p.HasEnded())
	{
		player->Update(p.GetDeltaTime());
		for (auto& bullet : bullets)
		{
			bullet->Update(p.GetDeltaTime());
		}

		p.BeginPaint();
		p.PaintBackground();
		p.PaintPlayer(player);

		for (auto&& enemy : enemies)
		{
			p.PaintEnemy(enemy);
		}

		for (auto&& bullet : bullets)
		{
			p.PaintBullet(bullet);
		}

		p.EndPaint();
	}
}