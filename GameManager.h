#pragma once
#include "ECS.h"

class Grid;
class BlockPool;
class PlayerController;
class Block;

class GameManager : public Component
{
public:
	bool validateMatches;

	GameManager();
	~GameManager();
	void init() override;
	void update() override;
	void setPlayerAndPool(Entity* entity);
	void addPlacedBlock(Block* block);
	
private:
	Grid* grid;
	BlockPool* blockPool;
	PlayerController* player;

	std::vector<Block*> activeBlocks;
	std::vector<Block*> matchedBlocks;

	bool repositioningBlocks;

	void validatedMatches();
	void updateScore(Block* blockA, Block* blockB);
};