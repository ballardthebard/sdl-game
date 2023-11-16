#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);
	void handleEvents();
	void start();
	void update();
	void render();
	void quit();

	bool running() { return isRunning; }
	static SDL_Renderer* renderer;

private:
	bool isRunning;
	SDL_Window* window;
};