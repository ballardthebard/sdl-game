#pragma once
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class Sprite : public Component
{
public:
	Sprite() = default;

	Sprite(const char* path)
	{
		setTexture(path);

		srcRect.h = 32;
		srcRect.w = 32;
		srcRect.x = 0;
		srcRect.y = 0;

		destRect.h = 32;
		destRect.w = 32;
		destRect.x = 0;
		destRect.y = 0;

		flip = SDL_FLIP_NONE;
	}

	Sprite(const char* path, int width, int height, int xpos, int ypos)
	{
		setTexture(path);

		srcRect.h = height;
		srcRect.w = width;
		srcRect.x = xpos;
		srcRect.y = ypos;

		destRect.h = height;
		destRect.w = width;
		destRect.x = xpos;
		destRect.y = ypos;

		flip = SDL_FLIP_NONE;
	}

	void setTexture(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<Transform>();
	}

	void update() override
	{
		if (transform == nullptr) return;

		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
		rotation = (int)transform->rotation;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, rotation, NULL, flip);
	}

	void setSize(int width, int height) {
		srcRect.h = height;
		srcRect.w = width;

		destRect.h = height;
		destRect.w = width;
	}

private:
	Transform* transform = nullptr;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_RendererFlip flip;
	int rotation;
};