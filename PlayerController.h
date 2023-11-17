#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"

class PlayerController : public Component
{
public:
	Transform* transform;

	void init() override
	{
		transform = &entity->getComponent<Transform>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_SPACE:
					transform->velocity.y = 1;
					break;
				case SDLK_RIGHT:
					transform->velocity.x = 1;
					break;
				case SDLK_LEFT:
					transform->velocity.x = -1;
					break;

			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_SPACE:
				transform->velocity.y = 0;
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 0;
				break;
			case SDLK_LEFT:
				transform->velocity.x = 0;
				break;

			default:
				break;
			}
		}
	}
};