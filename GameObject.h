#pragma once
#include "Game.h"

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren);
	~GameObject();

	void Start();
	void Update();
	void Render();

private:
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};