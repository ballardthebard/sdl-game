#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

// Generates a unique ID for each component type using a static counter
template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

// Track the existence of components in an entity
using ComponentBitSet = std::bitset<maxComponents>;
// Array of pointers to components in an entity
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
public:
	void update()
	{
		for (auto& c : components) c->update();
	}

	void draw()
	{
		for (auto& c : components) c->draw();
	}

	bool isDestroyed() const { return destroyed; }
	void destroy() { active = false; }

	bool isActive()const { return active; }
	void setActive(bool state) { active = state; }

	// Checks if the entity has a specific component of type T
	template<typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>];
	}

	// Adds a new component of type T to the entity
	template<typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	// Retrieves a component of type T
	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	bool active = true;
	bool destroyed = false;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
};

class Manager
{
public:
	// Updates all entities
	void update()
	{
		for (auto& e : entities)
		{
			if (e->isActive())
				e->update();
		}
	}

	// Draws all entities
	void draw()
	{
		for (auto& e : entities)
		{
			if (e->isActive())
				e->draw();
		}
	}

	// Removes inactive entities from the collection
	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return mEntity->isDestroyed();
			}),
			std::end(entities));
	}

	// Creates a new entity and adds it to the manager's collection
	Entity& addEntity()
	{
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
};