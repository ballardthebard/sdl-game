#pragma once
#include "Components.h"
#include "Color.h"

class Block : public Component
{
public:
	Color color;
	Transform* transform;

	bool isFalling;
	int fallInitialPosition = 0;
	int score = 0;

	Block(Color color)
	{
		this->color = color;
	}

	void init() override
	{
		transform = &entity->getComponent<Transform>();
	}
};