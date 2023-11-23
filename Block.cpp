#pragma once
#include "Block.h"
#include "Transform.h"

Block::Block(Color color)
{
	this->color = color;
}

Block::~Block() {}

void Block::init()
{
	transform = &entity->getComponent<Transform>();
}
