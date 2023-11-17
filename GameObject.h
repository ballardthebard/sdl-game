#pragma once
#include "Game.h"

class GameObject
{
public:
	GameObject(const char* texturesheet);
	~GameObject();

	void Start();
	void Update();
	void Render();

private:
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
};