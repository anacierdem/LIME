
#include "GameObjectManager.hpp"
#include "../Misc/Util.hpp"
#include <random>
#include "Renderer.hpp"
#include "Collider.hpp"

using namespace std;

QuadTree::QuadTree(AABB boundary)
{
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;

	points = vector< GameObject* >();
    this->boundary = boundary;
}

bool QuadTree::insert(GameObject* p) {
    // Ignore objects that do not belong in this quad tree
    if (!boundary.containsPoint(p->position))
        return false; // object cannot be added

    //If leaf node
    if (northWest == nullptr) {
        //Not full yet
        if(points.size() < nodeCapacity) {
            points.push_back(p);
            return true;
        }
        subdivide();
    }

    if (northWest->insert(p)) return true;
    if (northEast->insert(p)) return true;
    if (southWest->insert(p)) return true;
    if (southEast->insert(p)) return true;

    // Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
    return false;
}

void QuadTree::subdivide() {
    V2 size = boundary.halfDimension / 2.0f;
    V2 center = V2(boundary.center.x - size.x, boundary.center.y - size.y);
    northWest = new QuadTree(AABB(center, size));

    center = V2(boundary.center.x + size.x, boundary.center.y - size.y);
    northEast = new QuadTree(AABB(center, size));

    center = V2(boundary.center.x - size.x, boundary.center.y + size.y);
    southWest = new QuadTree(AABB(center, size));

    center = V2(boundary.center.x + size.x, boundary.center.y + size.y);
    southEast = new QuadTree(AABB(center, size));

    // Re-insert members
    for (int p = 0; p < points.size(); p++)
    {
        if (northWest->insert(points[p])) continue;
        if (northEast->insert(points[p])) continue;
        if (southWest->insert(points[p])) continue;
        southEast->insert(points[p]);
    }
    points.clear();
}

//Works in real coordinates (not screen projection or anything)
void QuadTree::queryRange(const AABB& range, vector<GameObject*>& list) const {

    // Automatically abort if the range does not intersect this quad
    if (!boundary.intersectsAABB(range))
        return; // empty list

    // Terminate here, if there are no children
    if (northWest == nullptr) {
        // Check objects at leaf level

        unsigned int pointsSize = points.size();
        unsigned int listSize = list.size();
		list.resize(listSize + pointsSize);

        unsigned int index = 0;
        for (int p = 0; p < pointsSize; p++)
        {
			if (boundary.containsPoint(points[p]->position)) {
                list[listSize + index] = points[p];
                index++;
            }
        }
        list.resize(listSize + index);

		return;
    } 

    // Otherwise, add the points from the children
	northWest->queryRange(range, list);
	northEast->queryRange(range, list);
	southWest->queryRange(range, list);
	southEast->queryRange(range, list);
}

QuadTree::~QuadTree()
{
	points.clear();

    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;
}

const int QuadTree::nodeCapacity = 200;

/*
 * GameObjectManager keeps track of objects in the scene with depth order
 * and holds map objects in a quad tree
 */
GameObjectManager::GameObjectManager(const sf::Texture* tex) : objects(AABB(V2(65536.0f, 65536.0f), V2(65536.0f, 65536.0f))) {
	auto generator = mt19937(15);

	int size = 10000;

	allObjects.resize(size);

	renderer = new Renderer(V2(192, 350), sf::Rect<int>(896, 1664, 384, 384));
	renderer->setTexture(*tex);

	collider = new Collider();

	for (int i = 0; i < size; i++) {

		unique_ptr<GameObject> tmpObj = make_unique<GameObject>(V2(131072.0f*generator() / generator.max(), 131072.0f*generator() / generator.max()), 0.4f + 1.2f*generator() / generator.max());
		
		tmpObj->addComponent(renderer);
		tmpObj->addComponent(collider);

		objects.insert(tmpObj.get());

		//allObjects manages the gameobjects
		allObjects[i] = move(tmpObj);
	}
}

void GameObjectManager::calculateVisibleObjects(sf::RenderWindow* window) {
    sf::Vector2i winSize(window->getSize());

    //Find positions coinciding to screen corners
    sf::Vector2f topLeft = Util::isoToTwoD(window->mapPixelToCoords(sf::Vector2i(0, 0)));
    sf::Vector2f center = (Util::isoToTwoD(window->mapPixelToCoords(sf::Vector2i(0, 0))) + Util::isoToTwoD(window->mapPixelToCoords(winSize))) / 2.0f;
    sf::Vector2f topRight = Util::isoToTwoD(window->mapPixelToCoords(sf::Vector2i(winSize.x, 0)));

    V2 halfDim = V2(center.x - topLeft.x, center.y - topRight.y);
	//TODO: make these sizes configurable
    halfDim.y = halfDim.y > 4000 ? 4000 : halfDim.y;
    halfDim.x = halfDim.x > 4000 ? 4000 : halfDim.x;

    //Find objects to render in twoD space (not screen)
    objects.queryRange(AABB(center, halfDim), currObjects);

    sf::Vector2f realCenter = (window->mapPixelToCoords(sf::Vector2i(0, 0)) + window->mapPixelToCoords(winSize)) / 2.0f;
    sf::Vector2f realHalfSize = (window->mapPixelToCoords(winSize) - window->mapPixelToCoords(sf::Vector2i(0, 0))) / 2.0f;
    realHalfSize += V2(512.0f, 512.0f);

    finerArea = AABB(realCenter, realHalfSize);
}

void GameObjectManager::update(sf::Time delta){
	// Update all game objects
	for (vector<GameObject*>::iterator it = currObjects.begin(); it != currObjects.end(); ++it) {
		if (finerArea.containsPoint(Util::twoDToIso((*it)->position)))
		{
			(*it)->update(delta);
		}
	}
}

void GameObjectManager::addObjects(vector<GameObject*>& objList)
{
	currObjects.insert(currObjects.end(), objList.begin(), objList.end());
}

void GameObjectManager::draw(sf::RenderWindow* window, sf::Shader* shader){

	//TODO: also draw unsorted game objects somewhere

	// Sort and draw all game objects
	sf::Shader::bind(shader);
	sort(currObjects.begin(), currObjects.end(), [](const GameObject* const a, const GameObject* const b) -> bool
	{
		return Util::compareDepth(a->position, b->position);
	});

	for (vector<GameObject*>::iterator it = currObjects.begin(); it != currObjects.end(); ++it) {
		if (finerArea.containsPoint(Util::twoDToIso((*it)->position)))
		{
			GameObject* tmpObj = *it;
			Renderer* renderer = tmpObj->getComponent<Renderer>();
			renderer->draw(window);
		}
	}

	sf::Shader::bind(nullptr);

	currObjects.clear();
}

bool GameObjectManager::isVisible(GameObject *object) {
    return finerArea.containsPoint(Util::twoDToIso(object->position));
}

void GameObjectManager::forEachVisibleItem(sf::RenderWindow* window, function<bool(GameObject *)> lambda) {
    calculateVisibleObjects(window);
    find_if(currObjects.begin(), currObjects.end(), lambda);
}

GameObjectManager::~GameObjectManager() {
	delete renderer;
}
