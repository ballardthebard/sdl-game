#pragma once
#include "Game.h"
#include "Components.h"

class PlayerController : public Component
{
public:
	void init() override
	{
		transform = &entity->getComponent<Transform>();
		blockPool = &entity->getComponent<BlockPool>();
	}

	void update() override
	{
		fall();

		inputEvents();
	}

	bool repositionBlocks(std::vector<Block*> activeBlocks)
	{
		bool isFalling = false;
		for (int i = 0; i < activeBlocks.size(); ++i)
		{
			if (!isFalling)
				isFalling = rapidFall(activeBlocks[i]);
			else
				rapidFall(activeBlocks[i]);
		}

		return isFalling;
	}

	void setGridAndManager(Entity* gridManager)
	{
		this->grid = &gridManager->getComponent<Grid>();
		this->gameManager = &gridManager->getComponent<GameManager>();
	}


	void setBlocks()
	{
		if (blockPool == nullptr) return;

		transform->position.x = 0;
		transform->position.y = 0;

		blockA = &blockPool->getEntity()->getComponent<Block>();
		blockB = &blockPool->getEntity()->getComponent<Block>();

		blockA->entity->setActive(true);
		blockB->entity->setActive(true);

		blockA->transform->position = transform->position;
		blockB->transform->position = transform->position;
		blockB->transform->position.x += tileWidth;

		blockA->transform->setParent(transform);
		blockB->transform->setParent(transform);

		canMove = true;
	}

private:
	BlockPool* blockPool;
	Grid* grid;
	GameManager* gameManager;

	Transform* transform;
	Block* blockA;
	Block* blockB;

	int tileWidth = 32;
	int tileHeight = 32;

	int fallInitialPosition;
	int fallVelocity = 3;
	float fallTime = 2;
	float fallCurrentTime = 0;

	bool canMove = true;

	void fallGrid()
	{

	}

	void fall()
	{
		bool aFall = rapidFall(blockA);
		bool bFall = rapidFall(blockB);

		if (aFall || bFall)
		{
			return;
		}
		else if (!canMove)
		{
			Vector2D gridPosA = grid->getGridPosition(blockA->transform);
			Vector2D gridPosB = grid->getGridPosition(blockB->transform);

			grid->tiles[gridPosA.x][gridPosA.y].block = blockA;
			grid->tiles[gridPosB.x][gridPosB.y].block = blockB;

			blockA->transform->setParent(nullptr);
			blockB->transform->setParent(nullptr);

			gameManager->validateMatches = true;
		}

		if (!blockA->isFalling && !blockB->isFalling)
			fallCurrentTime += Game::deltaTime;

		if (fallCurrentTime > fallTime)
		{
			fallCurrentTime = 0;

			if (grid->isObjectOnFreeTile(blockA->transform, 0, 1) && grid->isObjectOnFreeTile(blockB->transform, 0, 1))
			{
				transform->position.y += tileHeight;
			}
			else
			{
				canMove = false;
				blockA->isFalling = true;
				blockB->isFalling = true;
			}
		}
	}

	bool rapidFall(Block* block)
	{
		if (block->isFalling)
		{
			if (block->fallInitialPosition != 0)
			{
				if (block->transform->position.y - block->fallInitialPosition < tileHeight)
				{
					if (block->transform->hasParent())
						block->transform->localPosition.y += fallVelocity;
					else
						block->transform->position.y += fallVelocity;

					return true;
				}
				else if (grid->isObjectOnFreeTile(block->transform, 0, 1))
				{
					block->fallInitialPosition += tileHeight;
					return true;
				}
				else
				{
					int gridCorrection = block->fallInitialPosition - block->transform->position.y;

					if (block->transform->hasParent())
						block->transform->localPosition.y += gridCorrection;
					else
						block->transform->position.y += gridCorrection;

					block->isFalling = false;
					canMove = false;
				}
			}
			else if (grid->isObjectOnFreeTile(block->transform, 0, 1))
			{
				block->fallInitialPosition = block->transform->position.y;
				return true;
			}

		}
		else if (block->fallInitialPosition != 0)
		{
			int gridCorrection = tileHeight - (block->transform->position.y - block->fallInitialPosition);

			if (block->transform->hasParent())
				block->transform->localPosition.y += gridCorrection;
			else
				block->transform->position.y += gridCorrection;

			block->isFalling = false;
			block->fallInitialPosition = 0;
		}

		return false;
	}

	void inputEvents()
	{
		if (canMove && Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_SPACE:
				blockA->isFalling = true;
				blockB->isFalling = true;
				break;
			case SDLK_RIGHT:
				if (grid->isObjectOnFreeTile(blockA->transform, 1, 0) && grid->isObjectOnFreeTile(blockB->transform, 1, 0))
				{
					transform->position.x += tileWidth;
				}
				break;
			case SDLK_LEFT:
				if (grid->isObjectOnFreeTile(blockA->transform, -1, 0) && grid->isObjectOnFreeTile(blockB->transform, -1, 0))
				{
					transform->position.x -= tileWidth;
				}
				break;
			case SDLK_UP:
				if (blockA->transform->position.x < blockB->transform->position.x && grid->isObjectOnFreeTile(blockB->transform, -1, -1))
				{
					transform->rotation -= 90;
				}
				else if (blockA->transform->position.x > blockB->transform->position.x && grid->isObjectOnFreeTile(blockB->transform, 1, 1))
				{
					transform->rotation -= 90;

				}
				else if (blockA->transform->position.y < blockB->transform->position.y && grid->isObjectOnFreeTile(blockB->transform, 1, -1))
				{
					transform->rotation -= 90;
				}
				else if (blockA->transform->position.y > blockB->transform->position.y && grid->isObjectOnFreeTile(blockB->transform, -1, 1))
				{
					transform->rotation -= 90;
				}
				break;
			case SDLK_DOWN:

				if (blockA->transform->position.x < blockB->transform->position.x && grid->isObjectOnFreeTile(blockB->transform, -1, 1))
				{
					transform->rotation += 90;
				}
				else if (blockA->transform->position.x > blockB->transform->position.x && grid->isObjectOnFreeTile(blockB->transform, 1, -1))
				{
					transform->rotation += 90;
				}
				else if (blockA->transform->position.y < blockB->transform->position.y && grid->isObjectOnFreeTile(blockB->transform, -1, -1))
				{
					transform->rotation += 90;
				}
				else if (blockA->transform->position.y > blockB->transform->position.y && grid->isObjectOnFreeTile(blockB->transform, 1, 1))
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
				blockA->isFalling = false;
				blockA->isFalling = false;
				break;
			default:
				break;
			}
		}
	}
};