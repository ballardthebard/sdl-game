#pragma once
#include "Game.h"
#include "ECS.h"

class BlockPool;
class Grid;
class GameManager;
class Transform;
class Block;

class PlayerController : public Component
{
public:
	PlayerController();
	~PlayerController();
	void init() override;
	void update() override;
	bool repositionBlocks(std::vector<Block*> activeBlocks);
	void setGridAndManager(Entity* gridManager);
	void setBlocks();

private:
	BlockPool* blockPool;
	Grid* grid;
	GameManager* gameManager;

	Block* parentBlock;
	Block* childBlock;

	int ocupiedTiles = 0;

	int fallVelocity = 8;
	float fallTime = 1;
	float fallCurrentTime = 0;

	bool canMove = false;
	bool blocksPlaced = false;
	bool parentPlaced = false;
	bool childPlaced = false;

	void fall();
	void rapidFall(Block* block);
	void inputEvents();
	void updateGridAndManager(Block* block);
};