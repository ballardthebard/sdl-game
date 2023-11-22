#pragma once
#include "Components.h"

class BlockPool : public EntityPool
{
public:
	void initPool(EntityManager* entityManager, int poolSize)  override
	{
		EntityPool::initPool(entityManager, poolSize);

		for (int i = 0; i < entityPool.size(); ++i) {
			entityPool[i]->addComponent<Transform>();

			if (i < poolSize / 4)
			{
				entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Red.png");
				entityPool[i]->addComponent<Block>(RED);
			}
			else if (i < 2 * poolSize / 4)
			{
				entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Green.png");
				entityPool[i]->addComponent<Block>(GREEN);
			}
			else if (i < 3 * poolSize / 4)
			{
				entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Blue.png");
				entityPool[i]->addComponent<Block>(BLUE);
			}
			else
			{
				entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Yellow.png");
				entityPool[i]->addComponent<Block>(YELLOW);
			}
		}
	}
};