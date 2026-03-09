#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "InputManager.h"
#include "GameManager.h"
#include "PainterManager.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	InputManager *inputManager = new InputManager();
	PainterManager* painterManager = new PainterManager();
	Pool<Bullet, BULLETS_POOL_SIZE> bulletsPool;
	Pool<Plane, PLANES_POOL_SIZE> enemiesPool;

	Plane *player = new Plane();
	GameManager *gm = new GameManager(inputManager, player, &enemiesPool, &bulletsPool, painterManager);

	float deltaTime = 0;

	while (true)
	{
		const auto start_frame{std::chrono::steady_clock::now()};

		gm->Update(deltaTime);
		gm->Paint();
		painterManager->Paint();

		const auto end_frame{std::chrono::steady_clock::now()};
		const std::chrono::duration<float> elapsed_seconds{end_frame - start_frame};
		deltaTime = elapsed_seconds.count();
		//printf("Total Frames: %f\n", 1.0f/deltaTime);
	}

	delete inputManager;
	delete painterManager;
	delete player;
}