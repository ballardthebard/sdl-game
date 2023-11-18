#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"

class PlayerController : public Component
{
public:
	Transform* transform;
	Vector2D velocity;

	int speed = 3;

	void init() override
	{
		transform = &entity->getComponent<Transform>();
	}

	void update() override
	{
		transform->position.x += velocity.x * speed;
		transform->position.y += velocity.y * speed;

		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_SPACE:
					velocity.y = 1;
					break;
				case SDLK_RIGHT:
					velocity.x = 1;
					break;
				case SDLK_LEFT:
					velocity.x = -1;
					break;
				case SDLK_UP:
					transform->rotation -= 90;
					break;
				case SDLK_DOWN:
					transform->rotation += 90;
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
				velocity.y = 0;
				break;
			case SDLK_RIGHT:
				velocity.x = 0;
				break;
			case SDLK_LEFT:
				velocity.x = 0;
				break;

			default:
				break;
			}
		}
	}
};