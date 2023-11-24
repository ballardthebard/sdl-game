#include "Game.h"
#include "TextureManager.h"
#include "Grid.h"
#include "GameManager.h"
#include "Sprite.h"
#include "PlayerController.h"
#include "BlockPool.h"
#include "Transform.h"

EntityManager entityManager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
float Game::deltaTime = 0;

auto& grid(entityManager.addEntity());
auto& player(entityManager.addEntity());

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
	grid.addComponent<GameManager>();
	grid.addComponent<Sprite>("Assets/Sprites/T_Grid.png", 256, 512, 0, 0);

	player.addComponent<BlockPool>();
	player.addComponent<PlayerController>();

	player.getComponent<BlockPool>().initPool(&entityManager, 384);
	player.getComponent<PlayerController>().setBlocks();

	grid.getComponent<GameManager>().setPlayerAndPool(&player);
	player.getComponent<PlayerController>().setGridAndManager(&grid);

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
	entityManager.refresh();
	entityManager.update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	entityManager.draw();
	SDL_RenderPresent(renderer);
}

void Game::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Quit!" << std::endl;
}