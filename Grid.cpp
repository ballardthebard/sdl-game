#include "Grid.h"
#include "Transform.h"

Grid::Grid() {}
Grid::~Grid() {}

void Grid::init()
{
	initialPosition.x = 0;
	initialPosition.y = 0;
	tileSize.x = 32;
	tileSize.y = 32;
	numTiles.x = 8;
	numTiles.y = 16;

	lowestFreeTile.resize(numTiles.x);
	initializeTilesVector();
	setLowestFreeTile();
}

bool Grid::isObjectOnFreeTile(const Transform* obj, int xOffset, int yOffset) {
	if (obj == nullptr) return false; // Handle null pointer case

	Vector2D tile = getGridPosition(obj);

	tile.x += xOffset;
	tile.y += yOffset;

	// Check if the calculated indices are within the valid range
	if (tile.x >= 0 && tile.x < numTiles.x && tile.y >= 0 && tile.y < numTiles.y)
	{
		// Check if the tile is occupied
		if (tiles[tile.x][tile.y].isOccupied) {
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

void Grid::setLowestFreeTile()
{
	for (int x = 0; x < numTiles.x; x++)
	{
		for (int y = numTiles.y - 1; y >= 0; y--)
		{
			if (!tiles[x][y].isOccupied)
			{
				lowestFreeTile[x] = y;
				break;
			}
		}
	}
}

Vector2D Grid::getGridPosition(const Transform* obj)
{
	// Calculate the tile indices based on the object's position
	Vector2D tile;
	tile.x = static_cast<int>((obj->position.x - initialPosition.x) / tileSize.x);
	tile.y = static_cast<int>((obj->position.y - initialPosition.y) / tileSize.y);

	return tile;
}

Vector2D Grid::getGridPosition(Vector2D position)
{
	// Calculate the tile indices based on the object's position
	Vector2D tile;
	tile.x = static_cast<int>((position.x - initialPosition.x) / tileSize.x);
	tile.y = static_cast<int>((position.y - initialPosition.y) / tileSize.y);

	return tile;
}

bool Grid::isTileFree(Vector2D tile, int xOffset, int yOffset)
{
	if (tile.x >= 0 && tile.x < numTiles.x && tile.y >= 0 && tile.y < numTiles.y)
	{
		return tiles[tile.x + xOffset][tile.y + yOffset].isOccupied;
	}

	return false;
}

void Grid::occupyTile(Block* obj)
{
	Vector2D tilePos = getGridPosition(obj->transform);

	// Occupy the specified tile
	if (tilePos.x >= 0 && tilePos.x < numTiles.x && tilePos.y >= 0 && tilePos.y < numTiles.y)
	{
		tiles[tilePos.x][tilePos.y].isOccupied = true;
		tiles[tilePos.x][tilePos.y].block = obj;
	}
}

void Grid::freeColumn(int column)
{
	for (int i = 0; i < numTiles.y; ++i)
	{
		tiles[column][i].isOccupied = false;
		tiles[column][i].block = nullptr;
	}
}

void Grid::initializeTilesVector()
{
	// Initialize the tiles vector with the specified number of rows and columns
	for (int x = 0; x < numTiles.x; ++x) {
		std::vector<Tile> column;
		for (int y = 0; y < numTiles.y; ++y) {
			column.push_back(Tile());
		}
		tiles.push_back(column);
	}
}