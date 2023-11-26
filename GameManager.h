#pragma once
#include "ECS.h"

class Grid;
class BlockPool;
class PlayerController;
class Vector2D;
class Block;

class GameManager : public Component
{
public:
	bool validateMatches;
	std::vector<Block*> activeBlocks;

	GameManager();
	~GameManager();
	void init() override;
	void update() override;
	void setPlayerAndPool(Entity* entity);
	void addPlacedBlock(Block* block);
	void gameOver();

private:
	Grid* grid;
	BlockPool* blockPool;
	PlayerController* player;

	bool repositioningBlocks = false;

	void validatedMatches();
	void updateScoreRecursively(Block* block);
	std::vector<Block*> getSameColorNeighbors(Block* block);
};