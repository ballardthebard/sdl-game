#include "Transform.h"

Transform::Transform()
{
	position.x = 0;
	position.y = 0;
	rotation = 0;

	localPosition.x = 0;
	localPosition.y = 0;
	localRotation = 0;
}

Transform::Transform(Vector2D position, int rotation)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->rotation = rotation;

	localPosition.x = 0;
	localPosition.y = 0;
	localRotation = 0;
}

Transform::~Transform() {}

void Transform::update()
{
	if (parent == nullptr) return;

	// Set new position based on parent position
	this->position = parent->position + localPosition;

	if (parent->rotation != parentLastRotation)
	{
		// Set new position based on parent rotation
		this->position = rotateAroundParent(position, parent->position, parent->rotation - parentLastRotation);

		// Update localPosition
		Vector2D position = this->position;
		localPosition = position - parent->position;

		// Set new rotation according to parent
		this->rotation = parent->rotation + localRotation;

		// Update parent roation check
		parentLastRotation = parent->rotation;
	}
}

void Transform::setParent(const Transform* parent)
{
	if (parent == nullptr)
	{
		this->parent = nullptr;
		return;
	}

	this->parent = parent;

	Vector2D position = this->position;
	int rotation = this->rotation;

	localPosition = position - parent->position;
	localRotation = rotation - parent->rotation;
	parentLastRotation = parent->rotation;
}

bool Transform::hasParent()
{
	if (parent == nullptr) return false;

	return true;
}

// Rotate around parent by a given angle in degrees
Vector2D Transform::rotateAroundParent(Vector2D point, Vector2D center, double angleDegrees) {
	double angleRadians = angleDegrees * (M_PI / 180.0); // Convert degrees to radians
	double cosTheta = cos(angleRadians);
	double sinTheta = sin(angleRadians);

	// Translate the point relative to the center
	double translatedX = point.x - center.x;
	double translatedY = point.y - center.y;

	// Apply rotation formula around the center point
	double newX = translatedX * cosTheta - translatedY * sinTheta;
	double newY = translatedX * sinTheta + translatedY * cosTheta;

	// Deal with precision errors in floating-point arithmetic
	if (newX < EPSILON && newX > -EPSILON)
		newX = 0;
	if (newY < EPSILON && newY > -EPSILON)
		newY = 0;

	// Translate back to the original coordinates and return the rotated point
	Vector2D newPosition;
	newPosition.x = newX + center.x;
	newPosition.y = newY + center.y;

	return newPosition;
}