#include "GameManager.h"
#include "Grid.h"
#include "BlockPool.h"
#include "PlayerController.h"
#include "Block.h"

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::init()
{
	grid = &entity->getComponent<Grid>();
}

void GameManager::update()
{
	if (repositioningBlocks)
	{
		repositioningBlocks = player->repositionBlocks(activeBlocks);
	}
	else if (validateMatches)
	{
		validateMatches = false;
		validatedMatches();
	}
}

void GameManager::setPlayerAndPool(Entity* entity)
{
	player = &entity->getComponent<PlayerController>();
	blockPool = &entity->getComponent<BlockPool>();
}

void GameManager::addPlacedBlock(Block* block)
{
	activeBlocks.push_back(block);
}

void GameManager::validatedMatches()
{
	// Update scores and find matches
	for (int i = 0; i < activeBlocks.size(); ++i)
	{
		Vector2D gridPos = grid->getGridPosition(activeBlocks[i]->transform);

		if (gridPos.y - 1 >= 0)
		{
			updateScore(activeBlocks[i], grid->tiles[gridPos.x][gridPos.y - 1].block);

			if (activeBlocks[i]->score >= 4)
			{
				matchedBlocks.push_back(activeBlocks[i]);
				matchedBlocks.push_back(grid->tiles[gridPos.x][gridPos.y - 1].block);
			}
		}

		if (gridPos.y + 1 <= grid->numTiles.y - 1)
		{
			updateScore(activeBlocks[i], grid->tiles[gridPos.x][gridPos.y + 1].block);

			if (activeBlocks[i]->score >= 4)
			{
				matchedBlocks.push_back(activeBlocks[i]);
				matchedBlocks.push_back(grid->tiles[gridPos.x][gridPos.y + 1].block);
			}
		}

		if (gridPos.x - 1 >= 0)
		{
			updateScore(activeBlocks[i], grid->tiles[gridPos.x - 1][gridPos.y].block);

			if (activeBlocks[i]->score >= 4)
			{
				matchedBlocks.push_back(activeBlocks[i]);
				matchedBlocks.push_back(grid->tiles[gridPos.x - 1][gridPos.y].block);
			}
		}

		if (gridPos.x + 1 <= grid->numTiles.x - 1)
		{
			updateScore(activeBlocks[i], grid->tiles[gridPos.x + 1][gridPos.y].block);

			if (activeBlocks[i]->score >= 4)
			{
				matchedBlocks.push_back(activeBlocks[i]);
				matchedBlocks.push_back(grid->tiles[gridPos.x + 1][gridPos.y].block);
			}
		}
	}

	if (matchedBlocks.empty())
	{

	}
	else
	{
		// Remove matches from grid and return to blockPool
		for (int i = 0; i < matchedBlocks.size(); ++i)
		{
			Vector2D gridPos = grid->getGridPosition(matchedBlocks[i]->transform);

			grid->tiles[gridPos.x][gridPos.y].isOccupied = false;
			grid->tiles[gridPos.x][gridPos.y].block = nullptr;

			matchedBlocks[i]->entity->setActive(false);
			blockPool->addEntity(matchedBlocks[i]->entity);
		}

		matchedBlocks.clear();
		repositioningBlocks = true;
	}
}

void GameManager::updateScore(Block* blockA, Block* blockB)
{
	if (blockA->color == blockB->color)
	{
		if (blockA->score == 0)
		{
			blockA->score++;
			blockB->score++;
		}

		if (blockB->score == 0)
		{
			blockA->score++;
			blockB->score++;
		}

		if (blockA->score > blockB->score)
		{
			blockB->score = blockA->score;
		}
		else if (blockB->score > blockA->score)
		{
			blockA->score = blockB->score;
		}
	}
}