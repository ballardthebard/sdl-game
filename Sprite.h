#pragma once
#include "SDL.h"
#include "ECS.h"
#include "TextureManager.h"

class Transform;

class Sprite : public Component
{
public:
	Sprite();
	Sprite(const char* path);
	Sprite(const char* path, int width, int height, int xpos, int ypos);
	~Sprite();

	void setTexture(const char* path);
	void init() override;
	void update() override;
	void draw() override;
	void setSize(int width, int height);

private:
	Transform* transform = nullptr;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_RendererFlip flip;
	int rotation;
};