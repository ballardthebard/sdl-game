#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "Components.h"

class EntityPool : public Component {

public:
	void virtual initPool(EntityManager* entityManager, int poolSize)
	{
		for (int i = 0; i < poolSize; ++i) {
			Entity& entity = entityManager->addEntity();
			entityPool.push_back(&entity);
			entity.setActive(false);
		}
	}

	// Accessor to get an available entity from the pool
	Entity* getEntity()
	{
		// Generate random index
		int i = randomRange(0, entityPool.size());

		// No available entity in the pool
		if (entityPool[i] == nullptr) return nullptr;

		// Set entity to be returned
		Entity* block = entityPool[i];
		// Remove entity from pool
		entityPool.erase(entityPool.begin() + i);

		return block;
	}

	void addEntity(Entity* entity)
	{
		entityPool.push_back(entity);
	}

protected:

	std::vector<Entity*> entityPool;
	int randomRange(int min, int max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(min, max);
		return distribution(gen);
	}
};


