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

	void fall();
	bool rapidFall(Block* block);
	void inputEvents();

};