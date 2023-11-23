#pragma once
#include "ECS.h"
#include "Vector2D.h"

#define M_PI 3.14159265358979323846
#define EPSILON 1.192092896e-07F

class Transform : public Component
{
public:
	Vector2D position;
	Vector2D localPosition;
	int rotation;
	int localRotation;

	Transform();
	Transform(Vector2D position, int rotation);
	~Transform();

	void update() override;
	void setParent(const Transform* parent);
	bool hasParent();

private:
	const Transform* parent = nullptr;
	int parentLastRotation;

	Vector2D rotateAroundParent(Vector2D point, Vector2D center, double angleDegrees);
};