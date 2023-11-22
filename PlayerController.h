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
		blockPool = &entity->getComponent<BlockPool>();
	}

	void update() override
	{
		transform->position.y += velocity.y * speed;
		fall();
		rapidFall();
		inputEvents();
	}

	void setGridManager(Entity* gridManager)
	{
		this->grid = &gridManager->getComponent<Grid>();
		this->gameManager = &gridManager->getComponent<GameManager>();
	}

	void setBlocks()
	{
		if (blockPool == nullptr) return;

		blockA = &blockPool->getEntity()->getComponent<Transform>();
		blockB = &blockPool->getEntity()->getComponent<Transform>();

		blockA->entity->setActive(true);
		blockB->entity->setActive(true);

		blockA->position = transform->position;
		blockB->position = transform->position;
		blockB->position.x += tileWidth;

		blockA->setParent(transform);
		blockB->setParent(transform);

		aGridPos.x = 0;
		aGridPos.y = 0;
		bGridPos.x = 1;
		bGridPos.y = 0;
	}

private:
	BlockPool* blockPool;
	Grid* grid;
	GameManager* gameManager;

	Transform* transform;
	Vector2D velocity;
	Transform* blockA;
	Transform* blockB;

	Vector2D aGridPos;
	Vector2D bGridPos;

	int fallInitialPosition;
	bool isRapidFalling;

	float fallTime = 2;
	float fallCurrentTime = 0;

	bool canMove = true;

	void fall() 
	{
		if (!isRapidFalling)
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
	}

	void rapidFall()
	{
		if (isRapidFalling)
		{
			if (fallInitialPosition != 0)
			{
				if (transform->position.y - fallInitialPosition < tileHeight
					&& grid->isObjectOnTile(blockA, 0, 1)
					&& grid->isObjectOnTile(blockB, 0, 1))
				{
					velocity.y = 1;
				}
				else
				{
					aGridPos.y++;
					bGridPos.y++;

					if (grid->isObjectOnTile(blockA, 0, 1)
						&& grid->isObjectOnTile(blockB, 0, 1))
					{
						fallInitialPosition += tileHeight;
					}
					else
					{
						transform->position.y += fallInitialPosition - transform->position.y;

						velocity.y = 0;
						isRapidFalling = false;
					}
				}
			}
			else
			{
				if (grid->isObjectOnTile(blockA, 0, 1)
					&& grid->isObjectOnTile(blockB, 0, 1))
				{
					fallInitialPosition = transform->position.y;
				}
			}
		}
	}

	void inputEvents()
	{
		if (canMove && Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_SPACE:
				isRapidFalling = true;
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

		if (canMove && Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (isRapidFalling)
				{

					isRapidFalling = false;

					transform->position.y += tileHeight - (transform->position.y - fallInitialPosition);
					velocity.y = 0;

					aGridPos.y++;
					bGridPos.y++;
				}
				break;
			default:
				break;
			}
		}
	}
};