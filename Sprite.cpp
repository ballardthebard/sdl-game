#include "Sprite.h"
#include "Transform.h"

Sprite::Sprite() = default;

Sprite::Sprite(const char* path)
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

Sprite::Sprite(const char* path, int width, int height, int xpos, int ypos)
{
	setTexture(path);

	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = height;
	destRect.w = width;
	destRect.x = xpos;
	destRect.y = ypos;

	flip = SDL_FLIP_NONE;
}
Sprite::~Sprite() {}

void Sprite::setTexture(const char* path)
{
	texture = TextureManager::LoadTexture(path);
}

void Sprite::init()
{
	transform = &entity->getComponent<Transform>();
}

void Sprite::update()
{
	if (transform == nullptr) return;

	destRect.x = (int)transform->position.x;
	destRect.y = (int)transform->position.y;
	rotation = (int)transform->rotation;
}

void Sprite::draw()
{
	TextureManager::Draw(texture, srcRect, destRect, rotation, NULL, flip);
}

void Sprite::setSize(int width, int height) {
	srcRect.h = height;
	srcRect.w = width;

	destRect.h = height;
	destRect.w = width;
}