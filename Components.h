#pragma once
#include "ECS.h"

class Transform : public Component
{
public:
	int x() { return xpos; }
	int y() { return ypos; }


	void init() override
	{
		xpos = 0;
		ypos = 0;
	}

	void setPosition(int x, int y) {
		xpos = x;
		ypos = y;
	}


private:
	int xpos;
	int ypos;
};