#include "GameManager.h"
#include <queue>
#include <unordered_set> 
#include <algorithm>
#include <vector>
#include "Grid.h"
#include "BlockPool.h"
#include "PlayerController.h"
#include "Block.h"


GameManager::GameManager()
{
	activeBlocks = std::vector<Block*>();
}
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

// Function to add a block reference to activeBlocks
void GameManager::addPlacedBlock(Block* block) {
	auto it = std::find(activeBlocks.begin(), activeBlocks.end(), block);
	if (it == activeBlocks.end()) {
		activeBlocks.push_back(block);
	}
}

void GameManager::validatedMatches()
{
	for (int i = 0; i < activeBlocks.size(); ++i)
	{
		updateScoreRecursively(activeBlocks[i]);
	}

	repositioningBlocks = false;
	for (int i = 0; i < activeBlocks.size(); ++i)
	{
		if (activeBlocks[i]->score >= 4)
		{
			// Get current block grid position
			Vector2D gridPos = grid->getGridPosition(activeBlocks[i]->transform);

			// Return block to pool
			activeBlocks[i]->score = 0;
			activeBlocks[i]->entity->setActive(false);

			blockPool->addEntity(activeBlocks[i]->entity);

			// Remove block from grid
			grid->freeColumn(gridPos.x);

			// Remove block from activeBlocks
			activeBlocks.erase(activeBlocks.begin() + i);
			player->totalPlacedBlocks--;

			// Flags that blocks need repositioning
			repositioningBlocks = true;
		}
	}

	// If there are no matches, 
	if (!repositioningBlocks)
	{
		// Place the next pair of blocks
		player->setBlocks();
		std::cout << "Placed: " << player->totalPlacedBlocks << std::endl;
		std::cout << "Active: " << activeBlocks.size() << std::endl;
	}
}

void GameManager::updateScoreRecursively(Block* block)
{
	// Use a queue or stack for iterative BFS/DFS traversal
	std::queue<Block*> blocksToCheck;
	std::unordered_set<Block*> visited;

	blocksToCheck.push(block);
	visited.insert(block);

	while (!blocksToCheck.empty())
	{
		Block* currentBlock = blocksToCheck.front();
		blocksToCheck.pop();

		// Get the neighbors of the current block
		std::vector<Block*> neighbors = getSameColorNeighbors(currentBlock);

		for (Block* neighbor : neighbors)
		{
			if (visited.find(neighbor) == visited.end())
			{
				visited.insert(neighbor);
				blocksToCheck.push(neighbor);

				if (currentBlock->score == 0 && neighbor->score > 0)
				{
					neighbor->score++;
				}
				else if (currentBlock->score > 0 && neighbor->score == 0)
				{
					currentBlock->score++;
				}
				else if (currentBlock->score == 0 && neighbor->score == 0)
				{
					currentBlock->score = neighbor->score = 2;
				}
				if (currentBlock->score > neighbor->score)
				{
					neighbor->score = currentBlock->score;
				}
				else if (currentBlock->score < neighbor->score)
				{
					currentBlock->score = neighbor->score;
				}
			}
		}
	}
}

std::vector<Block*> GameManager::getSameColorNeighbors(Block* block)
{
	Vector2D gridPos = grid->getGridPosition(block->transform);
	std::vector<Block*> neighbors;

	if (gridPos.x - 1 >= 0) {
		Block* neighborBlock = grid->tiles[gridPos.x - 1][gridPos.y].block;
		if (neighborBlock != nullptr && block->color == neighborBlock->color) {
			neighbors.push_back(neighborBlock);
		}
	}

	if (gridPos.x + 1 < grid->numTiles.x) {
		Block* neighborBlock = grid->tiles[gridPos.x + 1][gridPos.y].block;
		if (neighborBlock != nullptr && block->color == neighborBlock->color) {
			neighbors.push_back(neighborBlock);
		}
	}

	if (gridPos.y - 1 >= 0) {
		Block* neighborBlock = grid->tiles[gridPos.x][gridPos.y - 1].block;
		if (neighborBlock != nullptr && block->color == neighborBlock->color) {
			neighbors.push_back(neighborBlock);
		}
	}

	if (gridPos.y + 1 < grid->numTiles.y) {
		Block* neighborBlock = grid->tiles[gridPos.x][gridPos.y + 1].block;
		if (neighborBlock != nullptr && block->color == neighborBlock->color) {
			neighbors.push_back(neighborBlock);
		}
	}

	return neighbors;
}