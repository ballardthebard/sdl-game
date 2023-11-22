#pragma once
#include "Components.h"

class GameManager : public Component
{
public:
	void init() override 
	{
		grid = &entity->getComponent<Grid>();
	}
	
	void validatedMatches() 
	{
	
	}

private:
	Grid* grid;
};