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
	int totalPlacedBlocks = 0;

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

	int tileWidth = 32;
	int tileHeight = 32;

	int ocupiedTiles = 0;

	int fallVelocity = 4;
	float fallTime = 1;
	float fallCurrentTime = 0;

	bool canMove = true;
	bool blocksPlaced = false;
	bool parentPlaced = false;
	bool childPlaced = false;

	void fall();
	void rapidFall(Block* block);
	void inputEvents();
	void updateGridAndManager(Block* block);

};