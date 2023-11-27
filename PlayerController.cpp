#include "PlayerController.h"
#include "BlockPool.h"
#include "Grid.h"
#include "GameManager.h"
#include "Transform.h"
#include "Block.h"

PlayerController::PlayerController() {}
PlayerController::~PlayerController() {}

void PlayerController::init()
{
	blockPool = &entity->getComponent<BlockPool>();
}

void PlayerController::update()
{
	inputEvents();
	fall();
}

void PlayerController::setGridAndManager(Entity* gridManager)
{
	this->grid = &gridManager->getComponent<Grid>();
	this->gameManager = &gridManager->getComponent<GameManager>();
}

void PlayerController::setBlocks()
{
	if (blockPool == nullptr) return;

	if (parentBlock != nullptr && childBlock != nullptr)
	{
		Vector2D parentGridPos = grid->getGridPosition(parentBlock->transform);
		Vector2D childGridPos = grid->getGridPosition(childBlock->transform);

		if (parentGridPos.y == 0 || childGridPos.y == 0)
		{
			blockPool->addEntity(parentBlock->entity);
			blockPool->addEntity(parentBlock->entity);

			parentBlock = nullptr;
			childBlock = nullptr;

			gameManager->gameOver();

			return;
		}
	}

	parentBlock = &blockPool->getEntity()->getComponent<Block>();
	parentBlock->transform->position.x = (grid->tileSize.x * grid->numTiles.x / 2) - grid->tileSize.x;
	parentBlock->transform->position.y = grid->initialPosition.y;
	parentBlock->transform->rotation = 0;

	childBlock = &blockPool->getEntity()->getComponent<Block>();
	childBlock->transform->position = parentBlock->transform->position;
	childBlock->transform->position.x += grid->tileSize.x;
	childBlock->transform->rotation = 0;

	childBlock->transform->setParent(parentBlock->transform);
	blocksPlaced = false;
	parentPlaced = false;
	childPlaced = false;

	parentBlock->entity->setActive(true);
	childBlock->entity->setActive(true);

	canMove = true;
}

bool PlayerController::repositionBlocks(std::vector<Block*> activeBlocks)
{
	bool isFalling = false;
	for (int i = 0; i < activeBlocks.size(); ++i)
	{
		activeBlocks[i]->isFalling = true;
		rapidFall(activeBlocks[i]);

		if (activeBlocks[i]->isFalling)
			isFalling = true;
		else
			grid->occupyTile(activeBlocks[i]);
	}
	// Tell GameManager to validate matches if no blocks are falling
	gameManager->validateMatches = !isFalling;

	return isFalling;
}

void PlayerController::updateGridAndManager(Block* block)
{
	grid->occupyTile(block);
	gameManager->addPlacedBlock(block);
}

void PlayerController::fall()
{
	// Return if blocks are not set
	if (parentBlock == nullptr || childBlock == nullptr) return;

	// Check if all blocks are placed
	if (!canMove && blocksPlaced)
	{
		// Reset controller
		parentBlock->transform->setParent(nullptr);
		childBlock->transform->setParent(nullptr);

		// Tell GameManager to validate matches
		gameManager->validateMatches = true;

		return;
	}

	// Check if blocks can move to the next row
	if (grid->isObjectOnFreeTile(parentBlock->transform, 0, 1) && grid->isObjectOnFreeTile(childBlock->transform, 0, 1))
	{
		// Update current fall time
		if (!parentBlock->isFalling && !childBlock->isFalling)
			fallCurrentTime += Game::deltaTime;

		// Reset fall time and move blocks to next row
		if (fallCurrentTime > fallTime)
		{
			fallCurrentTime = 0;
			parentBlock->transform->position.y += grid->tileSize.y - ((int)parentBlock->transform->position.y % (int)grid->tileSize.y);
		}
	}
	// If either block can't move to the next row
	else if (!blocksPlaced)
	{
		// Pause player control over blocks 
		canMove = false;
		if (!grid->isObjectOnFreeTile(parentBlock->transform, 0, 1) && !grid->isObjectOnFreeTile(childBlock->transform, 0, 1))
		{
			blocksPlaced = true;
		}
		// Check if parentBlock should be placed on grid
		if (!parentPlaced && !grid->isObjectOnFreeTile(parentBlock->transform, 0, 1))
		{
			parentPlaced = true;
			updateGridAndManager(parentBlock);
			childBlock->transform->setParent(nullptr);

			// Check if after placing parentBlock on grid, childBlock should still rapid fall
			if (!grid->isObjectOnFreeTile(childBlock->transform, 0, 1))
			{
				childPlaced = true;
				updateGridAndManager(childBlock);
			}
			else
				childBlock->isFalling = true;
		}
		// Check if childBlock should be placed on grid
		else if (!childPlaced && !grid->isObjectOnFreeTile(childBlock->transform, 0, 1))
		{
			childPlaced = true;
			updateGridAndManager(childBlock);
			childBlock->transform->setParent(nullptr);

			// Check if after placing childBlock on grid, parentBlock should still rapid fall
			if (!grid->isObjectOnFreeTile(parentBlock->transform, 0, 1))
			{
				parentPlaced = true;
				updateGridAndManager(parentBlock);
			}
			else
				parentBlock->isFalling = true;
		}
	}

	// Check if blocks should rapid fall
	rapidFall(parentBlock);
	rapidFall(childBlock);
}

void PlayerController::rapidFall(Block* block)
{
	if (block->isFalling)
	{
		if (grid->isObjectOnFreeTile(block->transform, 0, 1))
		{
			// Move block futher down
			if (!block->transform->hasParent())
				block->transform->position.y += fallVelocity;
		}
		else
		{
			// Stop rapid fall
			block->isFalling = false;
			// Pause player control over blocks 
			canMove = false;
		}
	}
}

void PlayerController::inputEvents()
{


	// Check if any key is pressed
	if (Game::event.type == SDL_KEYDOWN)
	{
		if (!gameManager->isGameStarted)
		{
			gameManager->isGameStarted = true;
			gameManager->gameStart();
		}

		// Check if player can give inputs
		if (!canMove) return;

		switch (Game::event.key.keysym.sym)
		{
		case SDLK_SPACE:
			// Rapid fall
			parentBlock->isFalling = true;
			childBlock->isFalling = true;

			break;

		case SDLK_RIGHT:
			// Check if tile to the right is free
			if (!parentBlock->isFalling && !childBlock->isFalling
				&& grid->isObjectOnFreeTile(parentBlock->transform, 1, 0) && grid->isObjectOnFreeTile(childBlock->transform, 1, 0))
			{
				// Move right
				parentBlock->transform->position.x += grid->tileSize.x;
			}
			break;
		case SDLK_LEFT:
			// Check if tile to the left is free
			if (!parentBlock->isFalling && !childBlock->isFalling
				&& grid->isObjectOnFreeTile(parentBlock->transform, -1, 0) && grid->isObjectOnFreeTile(childBlock->transform, -1, 0))
			{
				// Move left
				parentBlock->transform->position.x -= grid->tileSize.x;
			}
			break;

			// Rotate anti-clockwise
		case SDLK_UP:
			// Check if tile ABOVE is free
			if (parentBlock->transform->position.x < childBlock->transform->position.x && grid->isObjectOnFreeTile(childBlock->transform, -1, -1))
			{
				parentBlock->transform->rotation -= 90;
			}
			// Check if tile BELOW is free
			else if (parentBlock->transform->position.x > childBlock->transform->position.x && grid->isObjectOnFreeTile(childBlock->transform, 1, 1))
			{
				parentBlock->transform->rotation -= 90;
			}
			// Check if tile to the RIGHT is free
			else if (parentBlock->transform->position.y < childBlock->transform->position.y && grid->isObjectOnFreeTile(childBlock->transform, 1, -1))
			{
				parentBlock->transform->rotation -= 90;
			}
			// Check if tile to the LEFT is free
			else if (parentBlock->transform->position.y > childBlock->transform->position.y && grid->isObjectOnFreeTile(childBlock->transform, -1, 1))
			{
				parentBlock->transform->rotation -= 90;
			}
			break;

			// Rotate clockwise
		case SDLK_DOWN:
			// Check if tile ABOVE is free
			if (parentBlock->transform->position.x > childBlock->transform->position.x && grid->isObjectOnFreeTile(childBlock->transform, 1, -1))
			{
				parentBlock->transform->rotation += 90;
			}
			// Check if tile BELOW is free
			else if (parentBlock->transform->position.x < childBlock->transform->position.x && grid->isObjectOnFreeTile(childBlock->transform, -1, 1))
			{
				parentBlock->transform->rotation += 90;
			}
			// Check if tile to the RIGHT is free
			else if (parentBlock->transform->position.y > childBlock->transform->position.y && grid->isObjectOnFreeTile(childBlock->transform, 1, 1))
			{
				parentBlock->transform->rotation += 90;
			}
			// Check if tile to the LEFT is free
			else if (parentBlock->transform->position.y < childBlock->transform->position.y && grid->isObjectOnFreeTile(childBlock->transform, -1, -1))
			{
				parentBlock->transform->rotation += 90;
			}
			break;
		default:
			break;
		}
	}
	// Check if any key is released
	if (canMove && Game::event.type == SDL_KEYUP)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_SPACE:
			// Cancel rapid fall
			parentBlock->isFalling = false;
			childBlock->isFalling = false;
			break;
		default:
			break;
		}
	}
}