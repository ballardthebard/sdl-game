#include "BlockPool.h"

BlockPool::BlockPool(EntityManager* entityManager, int poolSize) : EntityPool(entityManager, poolSize)
{
	for (int i = 0; i < entityPool.size(); ++i) {
		entityPool[i]->addComponent<Transform>();

		if (i < poolSize / 4)
		{
			entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Red.png");
			entityPool[i]->addComponent<Block>(RED, 0);
		}
		else if (i < 2 * poolSize / 4)
		{
			entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Green.png");
			entityPool[i]->addComponent<Block>(GREEN, 0);
		}
		else if (i < 3 * poolSize / 4)
		{
			entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Blue.png");
			entityPool[i]->addComponent<Block>(BLUE, 0);
		}
		else
		{
			entityPool[i]->addComponent<Sprite>("Assets/Sprites/T_Block_Yellow.png");
			entityPool[i]->addComponent<Block>(YELLOW, 0);
		}
	}
}