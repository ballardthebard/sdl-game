#include "Game.h"

SDL_Texture* gridTexture;
SDL_Rect srcR, destR;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
	int flags = 0;
	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialized!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!" << std::endl;
		}
		isRunning = true;
	}

	SDL_Surface* tmpSurface = IMG_Load("Assets/Sprites/T_Grid.png");
	gridTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	destR.h = 512;
	destR.w = 256;
	destR.x = width/2 - 128;
	destR.y = height/2 - 256;

}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update()
{
}

void Game::render()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, gridTexture, NULL, &destR);

	SDL_RenderPresent(renderer);
}

void Game::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Quit!" << std::endl;
}