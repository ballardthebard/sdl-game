#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* filePath)
{
	SDL_Surface* tmpSurface = IMG_Load(filePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, int angle, SDL_Point* center, SDL_RendererFlip  flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, center, flip);
}
