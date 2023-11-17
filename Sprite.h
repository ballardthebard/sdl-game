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

		srcRect.h = 512;
		srcRect.w = 256;
		srcRect.x = 0;
		srcRect.y = 0;

		destRect.h = 512;
		destRect.w = 256;
		destRect.x = 0;
		destRect.y = 0;
	}

	void update() override
	{
		destRect.x = transform->x_position();
		destRect.y = transform->y_position();
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