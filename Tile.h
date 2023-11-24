#pragma once
#include "Block.h"

class Tile {
public:
    bool isOccupied;
    Block* block = nullptr;

    Tile() : isOccupied(false) {}
};