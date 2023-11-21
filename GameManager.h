#pragma once
#include "Components.h"

class GameManager
{
public:
	GameManager(Grid* grid);
	void validatedMatches();

private:
	Grid* grid;
};