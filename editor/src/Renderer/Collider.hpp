#pragma once

//Dependencies
#include <vector>
#include "../Misc/VectorExtension.hpp"
#include "../Core/GameObjectComponent.hpp"

//TODO: multiple collider types - use polymorpism instead?
//enum ColliderType
//{
//	POINT, VERTEX
//};

// A game object component that provides simple collision detection.
class Collider : public GameObjectComponent{

	std::vector<V2> collisionVertices;
	std::vector<V2> collisionVerticeNormals;
	
public:
	static sf::ConvexShape convex;

	Collider();
	Collider(Collider&);

	void draw(sf::RenderWindow* window); //Debug draw
	bool check(V2 position);
	V2 findNormal(V2 position);

	void addVertex(V2 position);
	void removeVertex(int index);
};

