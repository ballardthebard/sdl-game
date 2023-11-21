#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "Components.h"

class EntityPool {

public:
	EntityPool(EntityManager* entityManager, int poolSize);

	Entity* getEntity();

	void addEntity(Entity& entity);

protected:

	std::vector<Entity*> entityPool;
	int randomRange(int min, int max);
};


