#pragma once
#include "Components.h"
#include "Color.h"

class Block : public Component
{
public:
	int score = 0;
	Color color;

	Block(Color color, int score)
	{
		this->color = color;
		this->score = score;
	}
};