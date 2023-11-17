#pragma once
#include "Components.h"
#include "SDL.h"

class Sprite : public Component
{
public:
	Sprite() = default;
	Sprite(const char* path)
	{
		setTexture(path);
	}

	void setTexture(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<Transform>();

		srcRect.h = 32;
		srcRect.w = 32;
		srcRect.x = 0;
		srcRect.y = 0;

		destRect.h = 32;
		destRect.w = 32;
		destRect.x = 0;
		destRect.y = 0;
	}

	void update() override
	{
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}

private:
	Transform* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
};