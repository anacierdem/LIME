#pragma once

//Dependencies
#include <cmath>
#include <functional>
#include <memory>
#include <vector>

#include "../Misc/VectorExtension.hpp"

class GameObject;
class Renderer;
class Collider;

// Axis-aligned bounding box with half dimension and center
struct AABB
{
    V2 center;
    V2 halfDimension;

    AABB(V2 center = V2(), V2 halfDimension = V2()) : center(center), halfDimension(halfDimension){};

    bool containsPoint(const V2& a) const
    {
        return (std::abs(center.x - a.x) < halfDimension.x) &&
               (std::abs(center.y - a.y) < halfDimension.y);
    }

    bool intersectsAABB(const AABB& other) const
    {
        return (std::abs(center.x - other.center.x) < (halfDimension.x + other.halfDimension.x)) &&
               (std::abs(center.y - other.center.y) < (halfDimension.y + other.halfDimension.y));
    }
};

class QuadTree{

public:
    QuadTree() :
            northWest{ nullptr },
            northEast{ nullptr },
            southWest{ nullptr },
            southEast{ nullptr }
    {};

    QuadTree(AABB boundary);

	bool insert(GameObject* p);
    void subdivide(); // create four children that fully divide this quad into four quads of equal area
	void queryRange(const AABB& range, std::vector<GameObject*>& list) const;
    ~QuadTree();

private:
    // Arbitrary constant to indicate how many elements can be stored in this quad tree node
    static const int nodeCapacity;

    // Axis-aligned bounding box stored as a center with half-dimensions
    // to represent the boundaries of this quad tree
    AABB boundary;

	//TODO: keep these outside as a single vector & keep references only - Possible performance gain
    // Points in this quad tree node
	std::vector<GameObject*> points;

    // Children
    QuadTree* northWest;
    QuadTree* northEast;
    QuadTree* southWest;
    QuadTree* southEast;

    QuadTree(const QuadTree&);
    QuadTree& operator=(const QuadTree&);
};

// TODO: move AABB and QuadTree into their respective files.

// Manages all game objects in the scene including the player and trees
class GameObjectManager
{
    QuadTree objects;
    AABB finerArea;
	std::vector<std::unique_ptr<GameObject>> allObjects;
	std::vector<GameObject*> currObjects;

	//Temporary common renderer and collider
	Renderer* renderer;
	Collider* collider;

	void calculateVisibleObjects(sf::RenderWindow* window);

public:
	GameObjectManager(const sf::Texture *);
	~GameObjectManager();

	void update(sf::Time);
	void draw(sf::RenderWindow* window, sf::Shader* shader);
	void addObjects(std::vector<GameObject*>& objList);
	bool isVisible(GameObject* object);
	void forEachVisibleItem(sf::RenderWindow* window, std::function<bool(GameObject *)> lambda);
};
