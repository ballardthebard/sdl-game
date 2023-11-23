#pragma once
#include "ECS.h"
#include "Color.h"

class Transform;

class Block : public Component
{
public:
	Color color;
	Transform* transform;

	bool isFalling = false;
	int fallInitialPosition = 0;
	int score = 0;

	Block(Color color);
	~Block();

	void init() override;
};