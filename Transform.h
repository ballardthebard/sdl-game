#pragma once
#include "Components.h"
#include "Vector2D.h"

class Transform : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int speed = 3;

	Transform()
	{
		position.x = 0;
		position.y = 0;
	}

	Transform(Vector2D position)
	{
		this->position.x = position.x;
		this->position.y = position.y;
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};