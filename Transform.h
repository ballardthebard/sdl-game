#pragma once
#include "Components.h"

class Transform : public Component
{
public:
	int x_position() { return xpos; }
	int y_position() { return ypos; }

	Transform()
	{
		xpos = 0;
		ypos = 0;
	}

	Transform(int xpos, int ypos)
	{
		this->xpos = xpos;
		this->ypos = ypos;
	}

	void setPosition(int x, int y) {
		xpos = x;
		ypos = y;
	}

private:
	int xpos;
	int ypos;
};