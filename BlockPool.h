#pragma once
#include "EntityPool.h"

class BlockPool : public EntityPool
{
public:
	BlockPool(EntityManager* entityManager, int poolSize);
};