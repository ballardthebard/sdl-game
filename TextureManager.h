#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* filePath);
	static void  Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, int angle, SDL_Point* center, SDL_RendererFlip  flip);

private:
	SDL_Renderer* renderer;
};