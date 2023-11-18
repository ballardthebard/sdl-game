#pragma once
#include "Components.h"
#include "Vector2D.h"

class Transform : public Component
{
public:
	Vector2D position;
	Vector2D localPosition;
	int rotation;
	int localRotation;

	Transform()
	{
		position.x = 0;
		position.y = 0;
		rotation = 0;

		localPosition.x = 0;
		localPosition.y = 0;
		localRotation = 0;
	}

	Transform(Vector2D position, int rotation)
	{
		this->position.x = position.x;
		this->position.y = position.y;
		this->rotation = rotation;

		localPosition.x = 0;
		localPosition.y = 0;
		localRotation = 0;
	}

	void update() override
	{
		if (parent == nullptr) return;

		// Initialize local variables
		Vector2D parentPosition = parent->position;
		int parentRotation = parent->rotation;

		// Set new position based on parent position
		this->position = parentPosition + localPosition;

		if (parentRotation != parentLastRotation)
		{
			//Reset parent position
			parentPosition = parent->position;

			// Set new position based on parent rotation
			this->position = rotateAroundParent(position, parentPosition, parentRotation - parentLastRotation);

			// Update localPosition
			Vector2D position = this->position;
			localPosition = position - parentPosition;

			// Set new rotation according to parent
			this->rotation = parentRotation + localRotation;

			// Update parent roation check
			parentLastRotation = parent->rotation;
		}
	}

	void setParent(Transform* parent)
	{
		if (parent == nullptr) return;

		this->parent = parent;

		Vector2D position = this->position;
		int rotation = this->rotation;

		localPosition = position - parent->position;
		localRotation = rotation - parent->rotation;
		parentLastRotation = parent->rotation;
	}


private:
	Transform* parent = nullptr;
	int parentLastRotation;

	// Rotate around parent by a given angle in degrees
	Vector2D rotateAroundParent(Vector2D point, Vector2D center, double angleDegrees) {
		double angleRadians = angleDegrees * (M_PI / 180.0); // Convert degrees to radians
		double cosTheta = cos(angleRadians);
		double sinTheta = sin(angleRadians);

		// Translate the point relative to the center
		float translatedX = point.x - center.x;
		float translatedY = point.y - center.y;

		// Apply rotation formula around the center point
		float newX = translatedX * cosTheta - translatedY * sinTheta;
		float newY = translatedX * sinTheta + translatedY * cosTheta;

		// Translate back to the original coordinates and return the rotated point
		Vector2D newPosition;
		newPosition.x = newX + center.x;
		newPosition.y = newY + center.y;

		return newPosition;
	}
};