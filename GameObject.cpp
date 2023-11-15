#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, ren);
	
	srcRect.h = 512;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = 512;
	destRect.w = 256;
	destRect.x = 800 / 2 - 128;
	destRect.y = 600 / 2 - 256;
}

void GameObject::Start()
{
}

void GameObject::Update()
{

}

void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}