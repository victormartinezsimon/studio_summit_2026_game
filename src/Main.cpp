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
	GameManager *gm = new GameManager(inputManager, painterManager);

	float deltaTime = 0;

	auto lastTime = std::chrono::high_resolution_clock::now();

	while (true)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(currentTime - lastTime).count(); // seconds
    	lastTime = currentTime;

		gm->Update(deltaTime);
		gm->Paint();
		painterManager->Paint();
		//printf("Total Frames: %f\n", 1.0f/deltaTime);
	}

	delete inputManager;
	delete painterManager;
}