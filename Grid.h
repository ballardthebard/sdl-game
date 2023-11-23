#pragma once
#include <vector>
#include "Components.h"
#include "Tile.h"

class Grid : public Component {
public:
	std::vector<int> lowestFreeTile;
	std::vector<std::vector<Tile>> tiles;
	Vector2D numTiles;

	void init() override
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

	bool isObjectOnFreeTile(const Transform* obj, int xOffset, int yOffset) {
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

	void setLowestFreeTile()
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

	Vector2D getGridPosition(const Transform* obj)
	{
		// Calculate the tile indices based on the object's position
		Vector2D tile;
		tile.x = static_cast<int>((obj->position.x - initialPosition.x) / tileSize.x);
		tile.y = static_cast<int>((obj->position.y - initialPosition.y) / tileSize.y);

		return tile;
	}

	bool isTileFree(Vector2D tile, int xOffset, int yOffset)
	{
		if (tile.x >= 0 && tile.x < numTiles.x && tile.y >= 0 && tile.y < numTiles.y)
		{
			return tiles[tile.x + xOffset][tile.y + yOffset].isOccupied;
		}

		return false;
	}

	void occupyTile(int tileX, int tileY) {
		// Occupy the specified tile
		if (tileX >= 0 && tileX < numTiles.x && tileY >= 0 && tileY < numTiles.y) {
			tiles[tileX][tileY].isOccupied = true;
		}
	}

	void releaseTile(int tileX, int tileY) {
		// Release the specified tile
		if (tileX >= 0 && tileX < numTiles.x && tileY >= 0 && tileY < numTiles.y) {
			tiles[tileX][tileY].isOccupied = false;
		}
	}

private:
	Vector2D initialPosition;
	Vector2D tileSize;


	void initializeTilesVector()
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
};