#pragma once

//Dependencies
#include "../Misc/VectorExtension.hpp"

class GameObject;
class Collider;
class Controller;
class Renderer;

class GameObjectComponent;
class GameObjectManager;
enum GameObjectComponentType : unsigned int;

// Base class for game objects. Can have a collider, renderer and/or controller assigned.
class GameObject{
    friend class GameObjectComponent;
    friend class GameObjectManager;
    friend class QuadTree;

    friend class Renderer;
    friend class Collider;
    friend class Controller;
	
	Collider* collider = nullptr;
	Renderer* renderer = nullptr;
	Controller* controller = nullptr;

protected:
	//TODO: custom components
	//	std::vector<GameObjectComponent*> components;

    float scale;
    V2 position;
    V2 isoPosition;

public:

	GameObject();
	GameObject(V2 pos, float scale);
	GameObject(GameObject&);

	void addComponent(Collider* comp);
	void addComponent(Renderer* comp);
	void addComponent(Controller* comp);

    template <typename T>
    T* getComponent();

	V2 getPosition();
	void setPosition(V2);
	float getScale();
	void setScale(float);

    virtual void update(sf::Time delta);
    virtual void draw();

	~GameObject();
};

