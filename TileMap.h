#pragma once
#include <vector>
#include "Vector2D.h"
#include "Components.h"
#include "Tile.h"
#include "TileStatus.h"

class TileMap : public Component {
public:
	TileMap(Vector2D initialPos, Vector2D tileSize, int numTiles)
		: initialPosition(initialPos), tileSize(tileSize), numTilesX(numTiles), numTilesY(numTiles) {

		// Initialize the tiles vector with the specified number of rows and columns
		for (int y = 0; y < numTilesY; ++y) {
			std::vector<Tile> row;
			for (int x = 0; x < numTilesX; ++x) {
				row.push_back(Tile());
			}
			tiles.push_back(row);
		}
	}

	TileStatus isObjectOnTile(const Transform* obj, int xOffset, int yOffset) {
		if (obj == nullptr) return OUT_OF_BOUNDS; // Handle null pointer case

		// Calculate the tile indices based on the object's position
		int tileX = static_cast<int>((obj->position.x - initialPosition.x) / tileSize.x);
		int tileY = static_cast<int>((obj->position.y - initialPosition.y) / tileSize.y);

		tileX += xOffset;
		tileY += yOffset;

		// Check if the calculated indices are within the valid range
		if (tileX >= 0 && tileX < numTilesX && tileY >= 0 && tileY < numTilesY) {
			// Check if the tile is occupied
			if (tiles[tileY][tileX].isOccupied) {
				return OCCUPIED;
			}
			else {
				return FREE;
			}
		}
		return OUT_OF_BOUNDS;
	}

	void occupyTile(int tileX, int tileY) {
		// Occupy the specified tile
		if (tileX >= 0 && tileX < numTilesX && tileY >= 0 && tileY < numTilesY) {
			tiles[tileY][tileX].isOccupied = true;
		}
	}

	void releaseTile(int tileX, int tileY) {
		// Release the specified tile
		if (tileX >= 0 && tileX < numTilesX && tileY >= 0 && tileY < numTilesY) {
			tiles[tileY][tileX].isOccupied = false;
		}
	}

private:
	Vector2D initialPosition;
	Vector2D tileSize;
	int numTilesX;
	int numTilesY;
	std::vector<std::vector<Tile>> tiles;
};