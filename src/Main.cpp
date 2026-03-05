#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "LinuxPainter.h"

int main(int argc, char** argv)
{
	(void)argc; (void)argv;

	Painter* p = new LinuxPainter();

	Pool<Bullet, BULLETS_POOL_SIZE> bulletsPool;

	auto spawnPlayerBullet = [&bulletsPool](int index, Plane* p)
		{
			auto bullet = bulletsPool.Get();
			bullet->SetPosition(p->GetX(), p->GetY());
			bullet->SetVelocity(-10);
			bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
		};

	auto spawnEnemyBullet = [&bulletsPool](int index, Plane* p)
	{
			auto bullet = bulletsPool.Get();
			bullet->SetPosition(p->GetX(), p->GetY());
			bullet->SetVelocity(50);
			bullet->SetSize(BULLETS_WIDTH, BULLETS_HEIGHT);
	};

	Plane* player = new Plane();
	player->SetSize(PLANE_WIDTH, PLANE_HEIGHT);
	player->SetPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT*0.9);
	player->SetBulletsToFire(0);
	player->SetFireRate(2);


	Pool<Plane, PLANES_POOL_SIZE> enemiesPool;

	std::vector<Plane*> enemies;
	int totalEnemies = 4;
	for (int i = 0; i < totalEnemies; ++i)
	{
		auto enemy = enemiesPool.Get();
		int posX = SCREEN_WIDTH / (totalEnemies+1) * (i + 1);
		int posY = SCREEN_HEIGHT * 0.1;
		enemy->SetPosition(posX, posY);
		enemy->SetSize(ENEMY_WIDTH, ENEMY_HEIGHT);
		enemy->SetBulletsToFire(1);
		enemy->SetFireRate(2);
		enemy->SetCallbackFire(spawnEnemyBullet);
		enemies.push_back( enemy );
	}

	player->SetCallbackFire(spawnPlayerBullet);

	while (!p->HasEnded())
	{
		float deltaTime = p->GetDeltaTime();

		std::vector<Bullet*> bulletsToDelete;
		bulletsPool.for_each_active([deltaTime,&bulletsToDelete](Bullet& obj)
		{
			obj.Update(deltaTime);

			if (obj.GetY() < 0 || obj.GetY() > SCREEN_HEIGHT)
			{
				bulletsToDelete.push_back(&obj);
			}
		});

		for (auto&& bullet : bulletsToDelete)
		{
			bulletsPool.Release(bullet);
		}

		enemiesPool.for_each_active([deltaTime](Plane& obj)
		{
			obj.Update(deltaTime);
		});

		player->Update(p->GetDeltaTime());


		p->BeginPaint();
		p->PaintBackground();
		p->PaintPlayer(player);

		/*
		for (auto&& enemy : enemies)
		{
			p->PaintEnemy(enemy);
		}
		
		bulletsPool.for_each_active([&p](Bullet& bullet)
		{
			p->PaintBullet(&bullet);
		});
		*/
		
		p->EndPaint();
	}
}