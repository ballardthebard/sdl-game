#pragma once
#include "ECS.h"
#include "EntityPool.h"

class BlockPool : public EntityPool
{
public:
	BlockPool();
	~BlockPool();
	void initPool(EntityManager* entityManager, int poolSize)  override;
};