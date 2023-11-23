#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "ECS.h"

class EntityPool : public Component {

public:
	EntityPool();
	~EntityPool();
	void virtual initPool(EntityManager* entityManager, int poolSize);
	Entity* getEntity();
	void addEntity(Entity* entity);

protected:
	std::vector<Entity*> entityPool;
	int randomRange(int min, int max);
};


