#pragma once
#include "Game.h"
#include "Components.h"

class PlayerController : public Component
{
public:
	int speed = 3;
	int tileWidth = 32;
	int tileHeight = 32;

	void init() override
	{
		transform = &entity->getComponent<Transform>();
	}

	void update() override
	{
		transform->position.x += velocity.x * speed;
		transform->position.y += velocity.y * speed;

		fallCurrentTime += Game::deltaTime;
		if (fallCurrentTime > fallTime)
		{
			fallCurrentTime = 0;

			if (grid->lowestFreeTile[aGridPos.x] > aGridPos.y && grid->lowestFreeTile[bGridPos.x] > bGridPos.y)
			{
				transform->position.y += tileHeight;
				aGridPos.y++;
				bGridPos.y++;
			}
			else
			{
				canMove = false;
			}
		}

		if (canMove && Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (fallInitialPosition != 0)
				{
					if (transform->position.y - fallInitialPosition < tileHeight && grid->isObjectOnTile(transform, 0, 1))
					{
						velocity.y = 1;
					}
					else if (transform->position.y - fallInitialPosition >= tileHeight)
					{
						if (grid->isObjectOnTile(transform, 0, 1))
						{
							fallInitialPosition += tileHeight;
							aGridPos.y++;
							bGridPos.y++;
						}
						else
						{
							transform->position.y += tileHeight - (transform->position.y - fallInitialPosition);

							velocity.y = 0;
							aGridPos.y++;
							bGridPos.y++;
						}

					}
				}
				else
				{
					if (grid->isObjectOnTile(transform, 0, 1))
					{
						fallInitialPosition = transform->position.y;
					}
				}

				break;
			case SDLK_RIGHT:
				if (aGridPos.x + 1 < grid->lowestFreeTile.size() && bGridPos.x + 1 < grid->lowestFreeTile.size()
					&& grid->lowestFreeTile[aGridPos.x + 1] > aGridPos.y
					&& grid->lowestFreeTile[bGridPos.x + 1] > bGridPos.y)
				{
					transform->position.x += tileWidth;
					aGridPos.x++;
					bGridPos.x++;
				}
				break;
			case SDLK_LEFT:
				if (aGridPos.x - 1 >= 0 && bGridPos.x - 1 >= 0
					&& grid->lowestFreeTile[aGridPos.x - 1] > aGridPos.y
					&& grid->lowestFreeTile[bGridPos.x - 1] > bGridPos.y)
				{
					transform->position.x -= tileWidth;
					aGridPos.x--;
					bGridPos.x--;
				}
				break;
			case SDLK_UP:
				if (aGridPos.x < bGridPos.x
					&& bGridPos.x - 1 >= 0
					&& bGridPos.y - 1 >= 0
					&& grid->lowestFreeTile[bGridPos.x - 1] > bGridPos.y)
				{
					bGridPos.x--;
					bGridPos.y--;
					transform->rotation -= 90;

				}
				else if (aGridPos.x > bGridPos.x
					&& bGridPos.x + 1 < grid->lowestFreeTile.size()
					&& bGridPos.y + 1 < 16
					&& grid->lowestFreeTile[bGridPos.x + 1] > bGridPos.y)
				{
					bGridPos.x++;
					bGridPos.y++;
					transform->rotation -= 90;

				}
				else if (aGridPos.y < bGridPos.y
					&& bGridPos.x + 1 < grid->lowestFreeTile.size()
					&& bGridPos.y - 1 >= 0
					&& grid->lowestFreeTile[bGridPos.x + 1] >= bGridPos.y - 1)
				{
					bGridPos.x++;
					bGridPos.y--;
					transform->rotation -= 90;

				}
				else if (aGridPos.y > bGridPos.y
					&& bGridPos.x - 1 >= 0
					&& bGridPos.y + 1 < 16
					&& grid->lowestFreeTile[bGridPos.x - 1] >= bGridPos.y + 1)
				{
					bGridPos.x--;
					bGridPos.y++;
					transform->rotation -= 90;

				}
				break;
			case SDLK_DOWN:

				if (aGridPos.x < bGridPos.x
					&& bGridPos.x - 1 >= 0
					&& bGridPos.y + 1 < 16
					&& grid->lowestFreeTile[bGridPos.x - 1] >= bGridPos.y + 1)
				{
					transform->rotation += 90;
					bGridPos.x--;
					bGridPos.y++;
				}
				else if (aGridPos.x > bGridPos.x
					&& bGridPos.x + 1 < grid->lowestFreeTile.size()
					&& bGridPos.y - 1 >= 0
					&& grid->lowestFreeTile[bGridPos.x + 1] >= bGridPos.y - 1)
				{
					transform->rotation += 90;
					bGridPos.x++;
					bGridPos.y--;
				}
				else if (aGridPos.y < bGridPos.y
					&& bGridPos.x - 1 >= 0
					&& bGridPos.y + 1 < 16
					&& grid->lowestFreeTile[bGridPos.x - 1] > bGridPos.y)
				{
					transform->rotation += 90;
					bGridPos.x--;
					bGridPos.y--;
				}
				else if (aGridPos.y > bGridPos.y
					&& bGridPos.x + 1 < grid->lowestFreeTile.size()
					&& bGridPos.y + 1 < 16
					&& grid->lowestFreeTile[bGridPos.x + 1] > bGridPos.y)
				{
					transform->rotation += 90;
					bGridPos.x++;
					bGridPos.y++;
				}
				break;

			default:
				break;
			}
		}
	}

	void setBlocks(Transform* blockA, Transform* blockB)
	{
		this->blockA = blockA;
		this->blockB = blockB;

		this->blockA->position = transform->position;
		this->blockB->position = transform->position;
		this->blockB->position.x += tileWidth;

		this->blockA->setParent(transform);
		this->blockB->setParent(transform);

		aGridPos.x = 0;
		aGridPos.y = 0;
		bGridPos.x = 1;
		bGridPos.y = 0;
	}

	void setGrid(Grid* grid)
	{
		this->grid = grid;
	}

private:
	Transform* transform;
	Vector2D velocity;
	Transform* blockA;
	Transform* blockB;
	Grid* grid;

	Vector2D aGridPos;
	Vector2D bGridPos;

	int fallInitialPosition;

	float fallTime = 2;
	float fallCurrentTime = 0;

	bool canMove = true;
};