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

	parentBlock = &blockPool->getEntity()->getComponent<Block>();
	childBlock = &blockPool->getEntity()->getComponent<Block>();

	parentBlock->transform->position.x = 0;
	parentBlock->transform->position.y = 0;
	parentBlock->transform->rotation = 0;

	childBlock->transform->position = parentBlock->transform->position;
	childBlock->transform->position.x += tileWidth;
	childBlock->transform->rotation = 0;

	childBlock->transform->setParent(parentBlock->transform);

	parentBlock->entity->setActive(true);
	childBlock->entity->setActive(true);

	canMove = true;
	blocksPlaced = false;
	parentPlaced = false;
	childPlaced = false;
}

bool PlayerController::repositionBlocks(std::vector<Block*> activeBlocks)
{
	bool isFalling = false;
	for (int i = 0; i < activeBlocks.size(); ++i)
	{
		if (grid->isObjectOnFreeTile(activeBlocks[i]->transform, 0, 1))
		{
			isFalling = true;
			activeBlocks[i]->isFalling = true;
		}
		else if (!activeBlocks[i]->isFalling)
		{
			grid->occupyTile(activeBlocks[i]);
			//std::cout << "Occupied Tiles: " << ocupiedTiles << std::endl;
		}
		rapidFall(activeBlocks[i]);
	}

	return isFalling;
}

void PlayerController::updateGridAndManager(Block* block)
{
	totalPlacedBlocks++;
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
			parentBlock->transform->position.y += tileHeight - ((int)parentBlock->transform->position.y % tileHeight);
		}
	}
	// If either block can't move to the next row and canMove is true
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
	// Check if player can give inputs
	if (!canMove) return;

	// Check if any key is pressed
	if (Game::event.type == SDL_KEYDOWN)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_SPACE:
			// Rapid fall
			parentBlock->isFalling = true;
			childBlock->isFalling = true;
			break;

		case SDLK_RIGHT:
			// Check if tile to the right is free
			if (grid->isObjectOnFreeTile(parentBlock->transform, 1, 0) && grid->isObjectOnFreeTile(childBlock->transform, 1, 0))
			{
				// Move right
				parentBlock->transform->position.x += tileWidth;
			}
			break;
		case SDLK_LEFT:
			// Check if tile to the left is free
			if (grid->isObjectOnFreeTile(parentBlock->transform, -1, 0) && grid->isObjectOnFreeTile(childBlock->transform, -1, 0))
			{
				// Move left
				parentBlock->transform->position.x -= tileWidth;
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
	if (Game::event.type == SDL_KEYUP)
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