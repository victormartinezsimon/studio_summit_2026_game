#include <iostream>
#include <thread>
#include <chrono>

#include "GameConfig.h"
#include "Pool.h"
#include "Bullet.h"
#include "Plane.h"
#include "InputManager.h"
#include "GameManager.h"
#include "PainterManager.h"
#include "SoundManager.h"
#include "Painter.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	InputManager *inputManager = new InputManager();
	PainterManager* painterManager = new PainterManager();
	GameManager *gm = new GameManager(inputManager, painterManager);

	struct SPPlatform* platform = painterManager->GetPainter()->GetPlatform();

	SoundManager* soundManager = new SoundManager(platform);

	float deltaTime = 0;

	auto lastTime = std::chrono::high_resolution_clock::now();

	bool ended = false;

	soundManager->start();

	while (!ended)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(currentTime - lastTime).count(); // seconds
    	lastTime = currentTime;

		ended = gm->Update(deltaTime);
		gm->Paint();
		painterManager->Paint();
	}

	delete inputManager;
	delete painterManager;
	delete gm;
	delete soundManager;
}