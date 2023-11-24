#pragma once
#include <vector>
#include "ECS.h"
#include "Tile.h"
#include "Vector2D.h"

class Grid : public Component {
public:
	std::vector<int> lowestFreeTile;
	std::vector<std::vector<Tile>> tiles;
	Vector2D numTiles;

	Grid();
	~Grid();
	void init() override;
	bool isObjectOnFreeTile(const Transform* obj, int xOffset, int yOffset);
	void setLowestFreeTile();
	Vector2D getGridPosition(const Transform* obj);
	Vector2D getGridPosition(Vector2D position);
	bool isTileFree(Vector2D tile, int xOffset, int yOffset);
	void occupyTile(Block* obj);
	void freeColumn(int column);

private:
	Vector2D initialPosition;
	Vector2D tileSize;

	void initializeTilesVector();
};