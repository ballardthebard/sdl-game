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
		rapidFall(blockA);
		rapidFall(blockB);
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

	void rapidFall(Transform* blockTransform)
	{
		if (isRapidFalling)
		{
			if (fallInitialPosition != 0)
			{
				if (blockTransform->position.y - fallInitialPosition < tileHeight
					&& grid->isObjectOnFreeTile(blockTransform, 0, 1))
				{
					velocity.y = 1;
				}
				else
				{
					aGridPos.y++;
					bGridPos.y++;

					if (grid->isObjectOnFreeTile(blockTransform, 0, 1))
					{
						fallInitialPosition += tileHeight;
					}
					else
					{
						blockTransform->position.y += fallInitialPosition - blockTransform->position.y;

						velocity.y = 0;
						isRapidFalling = false;
					}
				}
			}
			else
			{
				if (grid->isObjectOnFreeTile(blockTransform, 0, 1))
				{
					fallInitialPosition = blockTransform->position.y;
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
			case SDLK_p:
				std::cout << "X: " << blockB->position.x << std::endl;
				std::cout << "Y: " << blockB->position.y << std::endl;
				break;
			case SDLK_SPACE:
				isRapidFalling = true;
				break;
			case SDLK_RIGHT:
				if (grid->isObjectOnFreeTile(blockA, 1, 0) && grid->isObjectOnFreeTile(blockB, 1, 0))
				{
					transform->position.x += tileWidth;
				}
				break;
			case SDLK_LEFT:
				if (grid->isObjectOnFreeTile(blockA, -1, 0) && grid->isObjectOnFreeTile(blockB, -1, 0))
				{
					transform->position.x -= tileWidth;
				}
				break;
			case SDLK_UP:
				if (blockA->position.x < blockB->position.x && grid->isObjectOnFreeTile(blockB, -1, -1))
				{
					transform->rotation -= 90;
				}
				else if (blockA->position.x > blockB->position.x && grid->isObjectOnFreeTile(blockB, 1, 1))
				{
					transform->rotation -= 90;

				}
				else if (blockA->position.y < blockB->position.y && grid->isObjectOnFreeTile(blockB, 1, -1))
				{
					transform->rotation -= 90;
				}
				else if (blockA->position.y > blockB->position.y && grid->isObjectOnFreeTile(blockB, -1, 1))
				{
					transform->rotation -= 90;
				}
				break;
			case SDLK_DOWN:

				if (blockA->position.x < blockB->position.x && grid->isObjectOnFreeTile(blockB, -1, 1))
				{
					transform->rotation += 90;
				}
				else if (blockA->position.x > blockB->position.x && grid->isObjectOnFreeTile(blockB, 1, -1))
				{
					transform->rotation += 90;
				}
				else if (blockA->position.y < blockB->position.y && grid->isObjectOnFreeTile(blockB, -1, -1))
				{
					transform->rotation += 90;
				}
				else if (blockA->position.y > blockB->position.y && grid->isObjectOnFreeTile(blockB, 1, 1))
				{
					transform->rotation += 90;
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
				}
				break;
			default:
				break;
			}
		}
	}
};