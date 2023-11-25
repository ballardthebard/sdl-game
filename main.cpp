#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 ticksCount = 0;
	float timeElapsed;

	game = new Game();
	game->init("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->running())
	{
		game->handleEvents();
		game->start();
		game->update();
		game->render();

		timeElapsed = SDL_GetTicks() - ticksCount;
		Game::deltaTime = timeElapsed / 1000.0f;
		ticksCount = SDL_GetTicks();

		if (frameDelay > timeElapsed)
		{
			SDL_Delay(frameDelay - timeElapsed);
		}
	}

	game->quit();

	delete game;

	return 0;
}