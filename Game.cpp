#include "Game.h"
#include "TextureManager.h"
#include "Components.h"

Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
float Game::deltaTime = 0;

auto& grid(manager.addEntity());
auto& player(manager.addEntity());
auto& block1(manager.addEntity());
auto& block2(manager.addEntity());

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

	grid.addComponent<Grid>();

	player.addComponent<Transform>();
	player.addComponent<PlayerController>();
	player.getComponent<PlayerController>().setGrid(&grid.getComponent<Grid>());

	block1.addComponent<Transform>();
	block1.getComponent<Transform>().position.x = 800 / 2 - 16;
	block1.getComponent<Transform>().position.y = 600 / 2 - 16;
	block1.addComponent<Sprite>("Assets/Sprites/T_Square_Red.png");

	block2.addComponent<Transform>();
	block2.getComponent<Transform>().position.x = 800 / 2 + 32 - 16;
	block2.getComponent<Transform>().position.y = 600 / 2 - 16;
	block2.addComponent<Sprite>("Assets/Sprites/T_Square_Blue.png");

	player.getComponent<PlayerController>().setBlocks(&block1.getComponent<Transform>(), &block2.getComponent<Transform>());

}

void Game::start()
{
}

void Game::handleEvents()
{

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
	manager.refresh();
	manager.update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Quit!" << std::endl;
}