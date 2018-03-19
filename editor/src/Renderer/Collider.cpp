
//Dependencies

#include "Collider.hpp"
#include "../Misc/Util.hpp"

Collider::Collider() {
	collisionVertices.push_back(V2(0.0f, -50.0f));
	collisionVertices.push_back(V2(80.0f, 20.0f));
	collisionVertices.push_back(V2(-80.0f, 20.0f));

	//TODO: move to calculateNormals()
	std::vector<int>::size_type sz = collisionVertices.size();
	for (unsigned int i = 0; i < sz; i++) {
		V2 currentNode = collisionVertices[i];
		V2 nextNode = collisionVertices[i < (sz - 1) ? (i + 1) : 0];
		V2 direction = (nextNode - currentNode);
		direction = direction.normalized(); //Normalize

		V2 norm(-direction.y, direction.x);
		collisionVerticeNormals.push_back(norm);
	}
}

void Collider::draw(sf::RenderWindow* window) {
	convex.setPointCount(3);
	convex.setPoint(0, collisionVertices[0]);
	convex.setPoint(1, collisionVertices[1]);
	convex.setPoint(2, collisionVertices[2]);

	convex.setFillColor(sf::Color::Red);

	convex.setPosition(Util::twoDToIso(owner->position));
	convex.setScale(owner->scale, owner->scale);

	window->draw(convex); //For debug
}

bool Collider::check(V2 position) {
	bool isCurrentObjectColliding = true;

	std::vector<V2> *vertices = &(collisionVertices);
	std::vector<int>::size_type sz = vertices->size();

	for (unsigned int i = 0; i < sz; i++) {
		V2 currentNode = (*vertices)[i] * owner->scale;
		V2 pointOnLine = currentNode + owner->isoPosition;

		V2 norm = collisionVerticeNormals[i];
		float result = (position - pointOnLine) * norm;
		//negative means outside (normals point towards inside)
		if (result < 0.0f) {
			isCurrentObjectColliding = false;
			break;
		}
	}
	return isCurrentObjectColliding;
}

V2 Collider::findNormal(V2 position) {
	std::vector<V2> *vertices = &(collisionVertices);
	std::vector<int>::size_type sz = vertices->size();

	float minDistance = std::numeric_limits<float>::max();
	V2 currentNorm;
	for (unsigned int i = 0; i < sz; i++) {
		V2 currentNode = (*vertices)[i];
		V2 pointOnLine = currentNode + owner->isoPosition;

		V2 norm = collisionVerticeNormals[i];
		float result = (position - pointOnLine) * norm;
		//negative means outside (normals point towards inside)
		if (result < minDistance) {
			minDistance = result;
			currentNorm = norm;
		}
	}

	return currentNorm;
}

void Collider::addVertex(V2 position)
{
	
}

void Collider::removeVertex(int index)
{
	
}

sf::ConvexShape Collider::convex;